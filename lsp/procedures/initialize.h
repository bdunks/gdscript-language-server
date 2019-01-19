/* initialize.h */

#ifndef LSP_PROCEDURES_INITIALIZE_H
#define LSP_PROCEDURES_INITIALIZE_H

#include "core/object.h"
#include "procedure.h"
#include "core/variant.h"

namespace lsp {
	class Initialize : public Procedure {

		GDCLASS(Initialize, Procedure);

	protected:
		static void _bind_methods();

	public:
		Variant run(const Variant &parameters) override;

	};
}

#endif // !LSP_PROCEDURES_INITIALIZE_H
