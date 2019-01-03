/* request.h */

#ifndef LSP_REQUEST_H
#define LSP_REQUEST_H

#include "message.h"

namespace lsp {
struct Request : Message {

	/* Request ID (int | String) */
	Variant id;

	/* Request Method to be invoked */
	String method;

	/* Method's parameters */
	Variant parameters;
};
} // namespace lsp

#endif // LSP_REQUEST_H
