/* iprocedure.h */

#ifndef LSP_IPROCEDURE_H
#define LSP_IPROCEDURE_H

#include "core/object.h"
#include "core/variant.h"
#include "../protocol/protocol.h"

namespace lsp {
class Procedure : public Object {
	GDCLASS(Procedure, Object);

public:
	virtual Variant run(const Variant &parameters) = 0;

};
}


#endif // !LSP_IPROCEDURE_H
