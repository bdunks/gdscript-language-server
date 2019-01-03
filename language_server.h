/* language_server.h */
#ifndef LANGUAGE_SERVER_H
#define LANGUAGE_SERVER_H

#include "core/io/packet_peer.h"
#include "core/io/tcp_server.h"
#include "core/object.h"
#include "core/os/mutex.h"
#include "core/os/thread.h"
#include "language_server_dispatcher.h"

namespace lsp {
class LanguageServer : public Object {
	GDCLASS(LanguageServer, Object);

private:
	static LanguageServer *singleton;
	static void thread_func(void *p_udata);

	Ref<TCP_Server> server;
	Ref<StreamPeerTCP> connection;
	LanguageServerDispatcher dispatcher;

	// Note - We are currently running the server in a single thread so the mutex, 
	// lock(), and unlock() are not used.  Will be used if requests are processed in 
	//sub-threads to manage queuing, etc.
	mutable bool exit_thread;
	Thread *thread;
	Mutex *mutex;

public:
	static LanguageServer *get_singleton();
	Error init();
	void lock();
	void unlock();
	void finish();
	String process_connection_data(String &connection_data);

	LanguageServer();
	~LanguageServer();
};
} // namespace lsp
#endif
