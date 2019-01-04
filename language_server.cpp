/* language_server.cpp */

#include "language_server.h"
#include "core/os/os.h"
#include "lsp/protocol/request.h"
#include "lsp/protocol/response.h"
#include "lsp/reader.h"
#include "lsp/writer.h"

namespace lsp {
String LanguageServer::process_connection_data(String &connection_data) {
	Request request;
	Response response;
	ResponseError err;
	err = Reader::parse_request(connection_data, request);
	if (err.code != OK) {
		return lsp::Writer::write_error_response(request, err);
	}

	err = dispatcher.dispatch(request, response);
	if (err.code != OK) {
		return Writer::write_error_response(request, err);
	}
	// Assume notification, no response expected.
	if (err.code == OK && response.result.is_zero()) {
		return Writer::write_no_response();
	}
	return Writer::write_response(response);
}

void LanguageServer::thread_func(void *p_udata) {

	auto *ls = reinterpret_cast<LanguageServer *>(p_udata);
	const uint64_t ms_delay = 1000;

	while (!ls->exit_thread) {

		// Delay at top of loop so it is hit if we've continued from an error condition
		OS::get_singleton()->delay_usec(ms_delay * 1000);

		if (ls->server->is_connection_available()) {
			ls->connection = ls->server->take_connection();
			if (ls->connection.is_null()) {
				continue;
			}
		} else {
			continue;
		}

		if (!ls->connection->is_connected_to_host()) {
			//Connection Broke
			continue;
		}
		const int bytes = ls->connection->get_available_bytes();
		String connection_data = ls->connection->get_utf8_string(bytes);
		if (connection_data.length() < 1) {
			//Ignore blank connection data sent by some clients as initial request
			continue;
		}

		String connection_response = ls->process_connection_data(connection_data);

		// Cannot directly put_string or it will be padded with additional data
		CharString utf = connection_response.utf8();
		ls->connection->put_data((const uint8_t *)utf.get_data(), utf.length());

		ls->connection->disconnect_from_host();
	}
}

Error LanguageServer::init() {
	server.instance();
	const int remote_port = 6071;
	Error err = server->listen(remote_port);
	if (err != Error::OK) {
		return err;
	}

	exit_thread = false;
	mutex = Mutex::create();
	thread = Thread::create(thread_func, this);
	return Error::OK;
}

void LanguageServer::finish() {
	if (!thread) {
		return;
	}

	exit_thread = true;
	Thread::wait_to_finish(thread);

	memdelete(thread);

	if (mutex) {
		memdelete(mutex);
	}

	thread = nullptr;
	server->stop();
}

void LanguageServer::unlock() {
	if (!thread || !mutex) {
		return;
	}

	mutex->unlock();
}

void LanguageServer::lock() {
	if (!thread || !mutex) {
		return;
	}

	mutex->lock();
}

LanguageServer *LanguageServer::singleton = nullptr;

LanguageServer *LanguageServer::get_singleton() {
	return singleton;
}

LanguageServer::LanguageServer() {
	singleton = this;
}

LanguageServer::~LanguageServer() {
	finish();
}
} // namespace lsp
