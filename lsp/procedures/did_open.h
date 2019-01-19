/* language_completion.h */

#ifndef LSP_PROCEDURES_DID_OPEN_H
#define LSP_PROCEDURES_DID_OPEN_H

#include "core/object.h"
#include "core/variant.h"
#include "procedure.h"

namespace lsp {
class DidOpen : public Procedure {

	GDCLASS(DidOpen, Procedure);

protected:
	static void _bind_methods();

public:
	Variant run(const Variant& parameters) override;

};
} // namespace lsp

#endif // !LSP_PROCEDURES_DID_OPEN_H
