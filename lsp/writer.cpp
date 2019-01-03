/* writer.cpp */

#include "writer.h"
#include "core/io/json.h"

namespace lsp {
String Writer::write_response(Response &response) {
	Dictionary resp_dict;
	resp_dict["jsonrpc"] = "2.0";
	resp_dict["id"] = response.id;
	resp_dict["result"] = response.result;
	String json = JSON::print(resp_dict);
	return json.json_escape();

	//TODO = throw in some HTTP Headers
};

String Writer::write_error_response(Request &request, ResponseError &error) {
	Dictionary error_dict;
	error_dict["code"] = error.code;
	error_dict["message"] = error.message;

	if (!error.data.is_zero()) {
		error_dict["data"] = error.data;
	}

	Dictionary resp_dict;
	resp_dict["jsonrpc"] = "2.0";
	resp_dict["id"] = request.id;
	resp_dict["error"] = error_dict;
	String json = JSON::print(resp_dict);
	return json.json_escape();

	//TODO = throw in some HTTP Headers
};
} // namespace lsp
