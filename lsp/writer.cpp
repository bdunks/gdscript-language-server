/* writer.cpp */

#include "writer.h"
#include "core/io/json.h"

namespace lsp {
String Writer::write_response(Response &response) {
	Dictionary resp_dict;
	resp_dict["jsonrpc"] = "2.0";
	resp_dict["id"] = response.id;
	resp_dict["result"] = response.result;
	String response_string = JSON::print(resp_dict);

	_add_header_part(response_string);
	return response_string;
}

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
	String response_string = JSON::print(resp_dict);

	_add_header_part(response_string);
	return response_string;
}

String Writer::write_no_response() {
	String header = "HTTP/1.1 204 No Content\r\n";
	header += "\r\n"; //- close header section
	return header;
}

String Writer::write_bad_request_response() {
	String header = "HTTP/1.1 400 Bad Request\r\n";
	header += "Accept: application/vscode-jsonrpc\r\n";
	header += "Accept-Charset: utf-8\r\n";
	header += "\r\n"; //- close header section
	return header;
}

void Writer::_add_header_part(String &string_resp) {
	String header = "Content-Length: " + itos(string_resp.length()) + "\r\n";
	header += "Content-Type: application/vscode-jsonrpc; charset=utf-8\r\n";
	header += "\r\n"; //- close header section

	string_resp = header + string_resp;
}
} // namespace lsp
