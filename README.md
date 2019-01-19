# gdscript-language-server
GDScript Language Server aspiring to implement the [LSP specification](https://microsoft.github.io/language-server-protocol/) as a Module for Godot.  

## To add to your Godot build:
1. Following the [Compiling Instructions](http://docs.godotengine.org/en/3.0/development/compiling/index.html) for your platform *with tools included* to verify you have a working build.
2. Add as a submodule to your godot source directory, e.g.:  `git submodule add https://github.com/bdunks/gdscript-language-server modules/gdscript-language_server`
3. Rebuild using the compilation steps from step #1
4. The server will begin accepting requests at http://localhost:6071 when the Godot Project Manager or Godot Editor starts

#### Editor Integrations

* [GDScript for Atom IDE (ide-gdscript)](https://github.com/bdunks/ide-gdscript) - Currently under development in parrallel with this language server

#### Available Procedures

| Procedure | Message Type | Status |
|------------|--------------|--------|
| [initialize](https://microsoft.github.io/language-server-protocol/specification#initialize)   | Request | Stub   |
| [Completion](https://microsoft.github.io/language-server-protocol/specification#textDocument_completion) | Request      | Stub   |
| [didOpen](https://microsoft.github.io/language-server-protocol/specification#textDocument_didOpen)   | Notification | Stub   |

## Contributing
The LSP protocol message structures have been defined within `./lsp/protocal/*`, and currently far outstrip the current implementation.  To increase coverage of the language server:

**Create a Procedure**

**Note:** When {procedure_name} / {ProcedureName} (in brackets) is used below, is meant to refer to the short name of the new procedure being added.  {procedure_name} referres to .h / .cpp file names, and {ProcedureName} refers to class names.

The JSON-RPC procedures for Notifications and Requests are created as subclasses to `./lsp/procedures/procedure.`
* Add a new class to `./lsp/procedures`
  *  Procedure handler's file and class names should match the *simplified* LSP Notification or Request Name (i.e., `did_open`, not `text_document_did_open`)  
  *  This is to avoid excessive namespacing of `text_document_` and `workspace_` when there are no conflicts in the current protocol.
* The class should inherit from `procedure`.  
* Register the class and its `run` method in the Godot ClassDB, which allows us to use Godot Variant::call("method") functionality in our dispatcher:
  * In the header file, include `GDCLASS({ProcedureName}, Procedure);` directly inside the class declaration
  * Override the protectected static method `_bind_methods()` to `ClassDB::bind_method(D_METHOD("run", "parameters"), &{ProcedureName}::invoke);`

**Procedure Structure**

The Procedure receives and return a [Godot Variant Object](https://docs.godotengine.org/en/latest/development/cpp/variant_class.html)
* Both the input and return structures can typically be represented as Dictionaries; however, they are defined as Variants for extra flexibility
* The `&parameters` are restricted to what is in the `params` key of the inbound RequestMessage or NotificationMessage.
  * The "jsonrpc" and "id" have will be re-applied automatically to the response
  * The "method" is implicit in the procedure being called
* The return structure should be restricted to the structure attached to the 'result' key

**Registering new Procedures**

1. Add two lines to `./language_server_dispatcher.cpp` to register the procedures
	* Preprocessor: `#include {procedure_name}.h;`
	* Constructor: `_register_procedure<lsp::{ProcedureName}>(String("{textDocument|workspace}/{ProcedureName}"));` 
2. If necessary, update `./lsp/proceedures/initialize.cpp` to register additional server capabilities with the client

## License

MIT License. See the [license](./LICENSE) for more details.
