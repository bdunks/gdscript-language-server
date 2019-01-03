#include "register_types.h"
#include "language_server.h"

#ifdef TOOLS_ENABLED

static lsp::LanguageServer *language_server = NULL;

void register_language_server_types() {
	language_server = memnew(lsp::LanguageServer);
    language_server->init();
}

void unregister_language_server_types() {
    if(language_server){
        language_server->finish();
        memdelete(language_server);
    }
}

#endif
