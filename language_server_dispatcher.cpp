/* language_server_dispatcher.cpp */

#include "language_server_dispatcher.h"
#include "lsp/protocol/request.h"
#include "lsp/protocol/response.h"

/* LSP Procedures */
#include "lsp/procedures/completion.h"
#include "lsp/procedures/did_open.h"
//...

namespace lsp {
ResponseError LanguageServerDispatcher::dispatch(Request &request, Response &ret_resp) {
	Variant procedure;
	ResponseError err = _get_procedure(request.method, procedure);

	if (err.code != OK) {
		return err;
	}

	Variant results = procedure.call("invoke", request.parameters);
	ret_resp = _create_response(request, results);
	return err;
};

template <typename T>
void LanguageServerDispatcher::_register_procedure(String &method) {
	//TODO - check procedure doesn't already exist
	//Wrap the procedure class in a variant so we can use the ->call functionality
	if (procedures[method].is_zero()) {
		ClassDB::register_class<T>();
		const Variant procedure = memnew(T);
		procedures[method] = procedure;

	} else {
		ERR_PRINT("Attempted to register an LSP proceedure multiple times.  Only first proceedure is registered.");	
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
	// Text Synchronization Messages
	_register_procedure<DidOpen>(String("textDocument/didOpen"));
	_register_procedure<DidOpen>(String("textDocument/didOpen"));
	//... add more Text Sync messages here

	// Language Feature Messages
	_register_procedure<Completion>(String("textDocument/completion"));
	//... add more Language Feature Messages here
}
}; // namespace lsp
		;
