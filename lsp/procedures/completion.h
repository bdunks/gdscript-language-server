/* language_completion.h */

#ifndef LSP_PROCEDURES_COMPLETION_H
#define LSP_PROCEDURES_COMPLETION_H

#include "core/object.h"
#include "procedure.h"
#include "core/variant.h"

namespace lsp {
	class Completion : public Procedure {

		GDCLASS(Completion, Procedure);

	protected:
		static void _bind_methods();

	public:
		Variant invoke(const Variant &parameters);

	private:
		Dictionary _convert_completion_to_dict(CompletionList &completionList);
	};
}

#endif // !LSP_PROCEDURES_COMPLETION_H
