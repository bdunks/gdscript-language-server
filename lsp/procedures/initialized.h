/* language_completion.h */

#ifndef LSP_PROCEDURES_INITIALIZED_H
#define LSP_PROCEDURES_INITIALIZED_H

#include "core/object.h"
#include "core/variant.h"
#include "procedure.h"

namespace lsp {
class Initialized : public Procedure {

	GDCLASS(Initialized, Procedure);

protected:
	static void _bind_methods();

public:
	Variant run(const Variant &parameters) override;

};
} // namespace lsp

#endif // !LSP_PROCEDURES_INITIALIZED_H
