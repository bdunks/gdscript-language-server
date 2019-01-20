/* language_server.cpp */

#include "language_server.h"
#include "core/os/os.h"
#include "lsp/reader.h"
#include "lsp/writer.h"

namespace lsp {

void LanguageServer::_thread_listener(void *p_udata) {

	auto *ls = reinterpret_cast<LanguageServer *>(p_udata);
	const uint64_t ms_delay = 100;

	while (!ls->exit_listener_thread) {
		ls->_check_for_new_connection_request();
		ls->_process_open_connections();
		OS::get_singleton()->delay_usec(ms_delay * 1000);
		//Extend sleep when waiting for new connection to ensure post data is fully received
		if (ls->connections.empty()) {
			OS::get_singleton()->delay_usec(ms_delay * 1000 * 20);
		} else {
			OS::get_singleton()->delay_usec(ms_delay * 1000);
		}
	}
}

void LanguageServer::_check_for_new_connection_request() {

	if (!server->is_connection_available()) {
		return;
	}

	Ref<StreamPeerTCP> connection = server->take_connection();
	if (connection.is_null() || !connection->is_connected_to_host()) {
		return;
	}

	const int bytes = connection->get_available_bytes();
	String connection_data = connection->get_utf8_string(bytes);
	if (connection_data.length() < 1) {
		//Ignore blank connection data sent by some clients as initial request
		return;
	}

	// -- Attempt to launch new LSP Thread
	CharString utf_response;
	Dictionary json_request;
	ResponseError lsp_error = Reader::parse_json_request(connection_data, json_request);

	const String host = json_request["host"];
	const int port = json_request["port"];

	if (host.empty() || port == 0) {
		print_error("Received invalid request without host / port");
		utf_response = Writer::write_bad_request_response().utf8();
	}

	IP_Address ip;

	if (host.is_valid_ip_address()) {
		ip = host;
	} else {
		ip = IP::get_singleton()->resolve_hostname(host);
	}

	// Create a new Stream Peer based on the editor's requested port
	Ref<StreamPeerTCP> client;
	client.instance();

	print_line("Connecting to language server client: " + String(ip) + ":" + itos(port));
	Error err = client->connect_to_host(ip, port);

	if (client->get_status() == StreamPeerTCP::STATUS_CONNECTED) {
		_create_connection(client);
		utf_response = Writer::write_no_response().utf8();
	} else {
		print_error("Can't connect to IP: " + String(ip) + " port " + itos(port));
		utf_response = Writer::write_bad_request_response().utf8();
	}

	connection->put_data((const uint8_t *)utf_response.get_data(), utf_response.length());
	connection->disconnect_from_host();
}

void LanguageServer::_process_open_connections() {
	for (Set<RID>::Element *e = connections.front(); e; e = e->next()) {
		auto lsc = connection_owner.getornull(e->get());

		if (lsc) {
			if (lsc->connection->get_status() != StreamPeerTCP::STATUS_CONNECTED) {
				_delete_connection(e->get());
				continue;
			}
			const int bytes = lsc->connection->get_available_bytes();
			String connection_data = lsc->connection->get_utf8_string(bytes);
			if (connection_data.length() < 1) {
				continue;
			}

			String connection_response = get_singleton()->dispatcher->process_connection_data(connection_data);
			if (!connection_response.empty()) {
				// Explicitly convert to UTF8.  Using ->put_string() will padded with additional data.
				CharString utf = connection_response.utf8();
				lsc->connection->put_data((const uint8_t *)utf.get_data(), utf.length());
			}
		}
	}
}

RID LanguageServer::_create_connection(Ref<StreamPeerTCP> &connection) {
	lock();
	LanguageServerConnection *lsc = memnew(LanguageServerConnection(connection));
	RID rid = connection_owner.make_rid(lsc);
	connections.insert(rid);
	unlock();
	return rid;
}

bool LanguageServer::_delete_connection(RID id) {
	if (connection_owner.owns(id)) {
		lock();
		LanguageServerConnection *lsc = connection_owner.get(id);
		connection_owner.free(id);
		connections.erase(id);
		memdelete(lsc);
		unlock();
		return true;
	}
	return false;
}

Error LanguageServer::init() {
	dispatcher = memnew(LanguageServerDispatcher);
	server.instance();
	//Listen on port G-O-D-O-T.
	const int remote_port = 46368;
	Error err = server->listen(remote_port);
	if (err != Error::OK) {
		return err;
	}
	exit_listener_thread = false;
	mutex = Mutex::create();
	listener_thread = Thread::create(_thread_listener, this);
	return Error::OK;
}

void LanguageServer::finish() {
	if (!listener_thread) {
		return;
	}

	exit_listener_thread = true;

	Thread::wait_to_finish(listener_thread);

	memdelete(listener_thread);

	if (mutex) {
		memdelete(mutex);
	}

	memdelete(dispatcher);
	
	listener_thread = nullptr;
	server->stop();
}

void LanguageServer::unlock() {
	if (!listener_thread || !mutex) {
		return;
	}

	mutex->unlock();
}

void LanguageServer::lock() {
	if (!listener_thread || !mutex) {
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
