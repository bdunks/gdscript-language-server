/* language_server_dispatcher.cpp */

#include "language_server_dispatcher.h"
#include "lsp/protocol/request.h"
#include "lsp/protocol/response.h"
#include "lsp/reader.h"
#include "lsp/writer.h"

/* LSP Procedures */
#include "lsp/procedures/completion.h"
#include "lsp/procedures/did_open.h"
#include "lsp/procedures/initialize.h"
#include "lsp/procedures/initialized.h"
//...

namespace lsp {

String LanguageServerDispatcher::process_connection_data(String &connection_data) {
	Request request;
	Response response;
	ResponseError err;
	err = Reader::parse_rpc_request(connection_data, request);
	if (err.code != OK) {
		return Writer::write_error_response(request, err);
	}

	err = dispatch_request(request, response);
	if (err.code != OK) {
		return Writer::write_error_response(request, err);
	}
	// Assume notification, no response expected.
	if (err.code == OK && response.result.is_zero()) {
		return String();
	}

	return Writer::write_response(response);
}

ResponseError LanguageServerDispatcher::dispatch_request(Request &request, Response &ret_resp) {
	Variant procedure;
	ResponseError err = _get_procedure(request.method, procedure);

	if (err.code != OK) {
		return err;
	}

	Variant results = procedure.call("run", request.parameters);
	ret_resp = _create_response(request, results);
	return err;
};

template <typename T>
void LanguageServerDispatcher::_register_procedure(String &method) {
	//Wrap the procedure class in a variant so we can use the ->call functionality
	if (procedures[method].is_zero()) {
		ClassDB::register_class<T>();
		const Variant procedure = memnew(T);
		procedures[method] = procedure;

	} else {
		String err_message = "Attempted to register an LSP procedure multiple times on the ";
		err_message += method + " method.  Only the first procedure registered will be invoked.";
		ERR_PRINT(err_message.utf8());	
		}

};

ResponseError LanguageServerDispatcher::_get_procedure(String &method, Variant &ret_procedure) {
	ResponseError err;
	ret_procedure = procedures[method];

	if (ret_procedure.is_zero()) {
		err.code = METHOD_NOT_FOUND;
		err.message = "Method ";
		err.message += method;
		err.message += " not found";
	}
	return err;
};

Response LanguageServerDispatcher::_create_response(Request &request, Variant &results) {
	Response response;
	response.jsonrpc = request.jsonrpc;
	response.id = request.id;
	response.result = results;
	return response;
};

LanguageServerDispatcher::LanguageServerDispatcher() {
	_register_procedure<Initialize>(String("initialize"));
	_register_procedure<Initialized>(String("initialized"));

	// Text Synchronization Messages
	_register_procedure<DidOpen>(String("textDocument/didOpen"));
	//... add more Text Sync messages here

	// Language Feature Messages
	_register_procedure<Completion>(String("textDocument/completion"));
	//... add more Language Feature Messages here
}
}; // namespace lsp
		;
