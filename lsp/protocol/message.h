/* message.h */
#ifndef LSP_MESSAGE_H
#define LSP_MESSAGE_H

#include "core/variant.h"

namespace lsp {
struct Message {
	/* JSON RPC Version */
	String jsonrpc;
};
} // namespace lsp

#endif // LSP_MESSAGE_H
