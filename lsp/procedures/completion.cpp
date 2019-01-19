/* completion.cpp */

#include "completion.h"

namespace lsp {
Variant Completion::run(const Variant &parameters) {
	CompletionList completion_list;

	// Stub code until actual completion is implemented
	completion_list.isIncomplete = true;

	completion_list.items.append(_create_completion_item(String("stop()")));
	completion_list.items.append(_create_completion_item(String("start()")));
	completion_list.items.append(_create_completion_item(String("play()")));
	// End stub code

	return _convert_completion_to_dict(completion_list);
};

Dictionary Completion::_convert_completion_to_dict(CompletionList &completion_list) {
	Dictionary result;

	result["isIncomplete"] = completion_list.isIncomplete;
	result["items"] = completion_list.items;

	return result;
};

Dictionary Completion::_create_completion_item(String &item) {
	Dictionary completion_item;
	completion_item["label"] = item;
	return completion_item;
}

void Completion::_bind_methods() {
	ClassDB::bind_method(D_METHOD("run", "parameters"), &Completion::run);
}
} // namespace lsp
