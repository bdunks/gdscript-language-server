/* language_server_dispatcher.h */

#ifndef LANGUAGE_SERVER_DISPATCHER_H
#define LANGUAGE_SERVER_DISPATCHER_H

#include "lsp/protocol/request.h"
#include "lsp/protocol/response.h"
#include "lsp/procedures/procedure.h"
#include "core/variant.h"

namespace lsp {

class LanguageServerDispatcher {
public:
	LanguageServerDispatcher();
	~LanguageServerDispatcher();
	String process_connection_data(String &connection_data);
	ResponseError dispatch_request(Request &request, Response &ret_resp);

private:
	Dictionary procedures;
	ResponseError _get_procedure(String &method, Variant &ret_procedure);
	Response _create_response(Request &request, Variant &results);

	template <typename T>
	void _register_procedure(String &method);
};
}// namespace lsp

#endif // LANGUAGE_SERVER_DISPATCHER_H
