/* completion.cpp */

#include "did_open.h"

namespace lsp {
Variant DidOpen::run(const Variant &parameters) {
	// Stub -- Could potentially send a notification to Godot Editor

	// Return empty string to signal this is a notification;
	return String();
};


void DidOpen::_bind_methods() {
	ClassDB::bind_method(D_METHOD("run", "parameters"), &DidOpen::run);
}
} // namespace lsp
