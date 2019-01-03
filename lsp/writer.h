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
};
} // namespace lsp

#endif // JSONRPC_WRITER_H
