/* writer.h */

#ifndef LSP_WRITER_H
#define LSP_WRITER_H

#include "protocol/response.h"
#include "protocol/request.h"
#include "core/ustring.h"

namespace lsp {
class Writer {
public:
	static String write_response(Response &response);
	static String write_error_response(Request &request, ResponseError &error);
	static String write_no_response();
	static String write_bad_request_response();
private:
	static void _add_header_part(String &string_resp);
};
} // namespace lsp

#endif // JSONRPC_WRITER_H
