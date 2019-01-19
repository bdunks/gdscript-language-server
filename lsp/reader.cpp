/* reader.cpp */

#include "reader.h"
#include "core/io/json.h"

namespace lsp {
ResponseError Reader::parse_rpc_request(String &connection_data, Request &ret_req) {
	Dictionary json_object;
	ResponseError err = parse_json_request(connection_data, json_object);

	_populate_request(json_object, ret_req);
	err = _validate_request(ret_req);

	if (err.code != OK) {
		return err;
	}

	return err;
}

ResponseError Reader::parse_json_request(String &connection_data, Dictionary &ret_json_obj) {
	Variant json_object;
	ResponseError err = _get_json_object(connection_data, json_object);
	if (err.code != OK) {
		return err;
	}

	err = _validate_json_object(json_object);
	if (err.code != OK) {
		return err;
	}
	ret_json_obj = json_object;
	return err;
}

ResponseError Reader::_get_json_object(String &data, Variant &ret_obj) {
	// Per LSP specification, '\r\n\r\n' is a valid way to identify the header
	const int header_seperator = data.find("\r\n\r\n", 0);
	String message = data.right(header_seperator + 4);
	
	ResponseError err;
	Variant parsed_message;
	String error_string;
	int error_line;

	const Error godot_err = JSON::parse(message, ret_obj, error_string, error_line);
	if (godot_err != Error::OK) {
		err.code = PARSE_ERROR;
		err.message = "Parsing Error: " + error_string + "at line " + itos(error_line);
	}
	return err;
}

ResponseError Reader::_validate_json_object(Variant &object) {
	ResponseError err;

	if (object.get_type() != Variant::DICTIONARY) {
		err.code = PARSE_ERROR;
		err.message = "Parsing Error: Could not parse `params` to a Dictionary";
		return err;
		}

	return err;
};

void Reader::_populate_request(Dictionary &json_obj, Request &ret_req) {
	ret_req.jsonrpc = json_obj["jsonrpc"];
	ret_req.id = json_obj["id"]; //May be blank, which is valid for Notifications
	ret_req.method = json_obj["method"];
	ret_req.parameters = json_obj["params"];
}

ResponseError Reader::_validate_request(Request &ret_req) {
	ResponseError err;
	if (ret_req.method.empty()) {
		err.code = INVALID_REQUEST;
		err.message = "Message must specify a method";
		return err;
	}

	if (ret_req.jsonrpc != "2.0") {
		err.code = INVALID_REQUEST;
		err.message = "Message must specify jsonprc version	\"2.0\"";
		return err;
	}
	
	return err;
}
} // namespace lsp
