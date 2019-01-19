/* language_server.h */
#ifndef LANGUAGE_SERVER_H
#define LANGUAGE_SERVER_H

#include "core/rid.h"
#include "core/io/packet_peer.h"
#include "core/io/tcp_server.h"
#include "core/io/stream_peer_tcp.h"
#include "core/object.h"
#include "core/os/mutex.h"
#include "core/os/thread.h"
#include "language_server_dispatcher.h"

namespace lsp {

class LanguageServerConnection : public RID_Data {

public:
	Ref<StreamPeerTCP> connection;
	LanguageServerConnection(const Ref<StreamPeerTCP> &connection) :
			connection(connection){};
	~LanguageServerConnection() = default;
};

class LanguageServer : public Object {
	GDCLASS(LanguageServer, Object);

private:
	static LanguageServer *singleton;
	static void _thread_listener(void *p_udata);

	void _check_for_new_connection_request();
	void _process_open_connections();

	RID _create_connection(Ref<StreamPeerTCP> &connection);
	bool _delete_connection(RID id);


	Ref<TCP_Server> server;
	Ref<StreamPeerTCP> _to_delete_connection;
	LanguageServerDispatcher dispatcher;

	mutable bool exit_listener_thread;
	Thread *listener_thread;
	Mutex *mutex;

	RID_Owner<LanguageServerConnection> connection_owner;
	Set<RID> connections;

public:
	static LanguageServer *get_singleton();
	Error init();
	void lock();
	void unlock();
	void finish();


	LanguageServer();
	~LanguageServer();
};
} // namespace lsp
#endif
