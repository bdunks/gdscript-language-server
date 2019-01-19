/* reader.h */

#ifndef LSP_READER_H
#define LSP_READER_H

#include "protocol/request.h"
#include "core/variant.h"
#include "protocol/response.h"

namespace lsp {
class Reader {

public:
	static ResponseError parse_rpc_request(String &connection_data, Request &ret_req);
	static ResponseError parse_json_request(String &connection_data, Dictionary &ret_json_obj);


private:
	static ResponseError _get_json_object(String &data, Variant &ret_obj);
	static ResponseError _validate_json_object(Variant &object);
	static void _populate_request(Dictionary &json_obj, Request &ret_req);
	static ResponseError _validate_request(Request &ret_req);
};

} // namespace lsp

#endif //LSP_READER_H
