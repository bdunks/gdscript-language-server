/* notification.h */

#ifndef LSP_NOTIFICATION_H
#define LSP_NOTIFICATION_H

#include "message.h"

namespace lsp {
struct Notification : Message {

	/* Notification Method to be invoked */
	String method;

	/* Method's parameters */
	Variant parameters;
};
} // namespace lsp
#endif // LSP_NOTIFICATION_H
