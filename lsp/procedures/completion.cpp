/* completion.cpp */

#include "completion.h"

namespace lsp {
Variant Completion::invoke(const Variant &parameters) {
	CompletionList completion_list;

	// Stub code until actual completion is implemented
	completion_list.isIncomplete = true;
	completion_list.items.append("An item");
	completion_list.items.append("Another item");
	// End stub code

	return _convert_completion_to_dict(completion_list);
};

Dictionary Completion::_convert_completion_to_dict(CompletionList &completion_list){
	Dictionary result;

	result["isIncomplete"] = completion_list.isIncomplete;
	result["items"] = completion_list.items;

	return result;
};

void Completion::_bind_methods() {
	ClassDB::bind_method(D_METHOD("invoke", "parameters"), &Completion::invoke);
}
} // namespace lsp
