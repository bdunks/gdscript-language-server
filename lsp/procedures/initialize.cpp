/* initialize.cpp */

#include "initialize.h"

namespace lsp {
Variant Initialize::run(const Variant &parameters) {
	Dictionary results;
	Dictionary serverCapabilities;
	Dictionary completionCapabilities;

	completionCapabilities["resolveProvider"] = false;
	completionCapabilities["triggerCharacters"] = ".";

	serverCapabilities["completionProvider"] = completionCapabilities;

	results["capabilities"] = serverCapabilities;

	return results;
};


void Initialize::_bind_methods() {
	ClassDB::bind_method(D_METHOD("run", "parameters"), &Initialize::run);
}
} // namespace lsp
