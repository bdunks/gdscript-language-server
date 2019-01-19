/* completion.cpp */

#include "initialized.h"

namespace lsp {
Variant Initialized::run(const Variant &parameters) {
	// Stub -- Editor has successfully initialized configuration with language server

	// Return empty string to signal this is a message;
	return String();
};


void Initialized::_bind_methods() {
	ClassDB::bind_method(D_METHOD("run", "parameters"), &Initialized::run);
}
} // namespace lsp
