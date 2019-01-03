/* response.h */

#ifndef LSP_RESPONSE_H
#define LSP_RESPONSE_H

#include "message.h"

namespace lsp {
enum ErrorCode {
	/* JSON RPC Defined */
	PARSE_ERROR = -32700,
	INVALID_REQUEST = -32600,
	METHOD_NOT_FOUND = -32601,
	INVALID_PARAMS = -32602,
	INTERNAL_ERROR = -32603,
	/* Reserved for Server Errors */
	SERVER_ERROR_START = -32099,
	SERVER_ERROR_END = -32000,
	/* Defined by LSP */
	REQUEST_CANCELLED = -32800,
	CONTENT_MODIFIED = -32801,
	/* Defined for internal processing */
	OK = 0
};

struct ResponseError {

	/* Error code defined by JSON RPC */
	ErrorCode code = OK;

	/* String description of error */
	String message;

	/* Additional message data, if applicable */
	Variant data;
};

struct Response : Message {

	/* Request ID (int | String | null) */
	Variant id;

	/* Result of the method; can be empty in case of error */
	Variant result;

	/* Error if any */
	ResponseError error;
};

} // namespace lsp

#endif // LSP_RESPONSE_H
