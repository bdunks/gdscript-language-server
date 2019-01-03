/* completion.cpp */

#include "did_open.h"

namespace lsp {
Variant DidOpen::invoke(const Variant &parameters) {
	CompletionList completionList;

	// Stub -- Could potentially send a notification to Godot Editor

	// Return empty string to signal this is a message;
	return String();
};


void DidOpen::_bind_methods() {
	ClassDB::bind_method(D_METHOD("invoke", "parameters"), &DidOpen::invoke);
}
} // namespace lsp
