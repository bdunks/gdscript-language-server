# gdscript-language-server
GDScript Language Server aspiring to implement the [LSP specification](https://microsoft.github.io/language-server-protocol/) as a Module for Godot.  

## To add to your Godot build:
1. Following the [Compiling Instructions](http://docs.godotengine.org/en/3.0/development/compiling/index.html) for your platform *with tools included* to verify you have a working build.
2. Add as a submodule to your godot source directory, e.g.,:  `git submodule add https://github.com/bdunks/gdscript-language-server modules/language_server`
3. Rebuild using the compilation steps from step #1
4. The server will begin accepting requests at http://localhost:6071 when the Godot Project Manager or Godot Editor starts

#### Testing

After you've launched your Godot build, you can test the procedures in the command line using curl: `curl --data "{\"jsonrpc\":\"2.0\",\"method\":\"textDocument/completion\",\"id\":1,\"params\":{\"textDocument\":\"dummy td\",\"position\":\"dummy pos\",\"context\":\"dummy context\"}}" localhost:6071 --output -`

#### Available Procedures

| Procedure | Message Type | Status |
|------------|--------------|--------|
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
* Register the class and its `invoke` method in the Godot ClassDB, which allows us to use Godot Variant::call("method") functionality in our dispatcher:
  * In the header file, include `GDCLASS({ProcedureName}, Procedure);` directly inside the class declaration
  * Override the protectected static method `_bind_methods()` to `ClassDB::bind_method(D_METHOD("invoke", "parameters"), &{ProcedureName}::invoke);`

**Procedure Structure**

The Procedure receives and return a [Godot Variant Object](https://docs.godotengine.org/en/latest/development/cpp/variant_class.html)
* Both the input and return structures can typically be represented as Dictionaries; however, they are defined as Variants for extra flexibility
* The `&parameters` are restricted to what is in the `params` key of the inbound RequestMessage or NotificationMessage.
  * The "jsonrpc" and "id" have will be re-applied automatically to the response
  * The "method" is implicit in the procedure being called
* The return structure should be restricted to the structure attached to the 'result' key

**Registering new Procedures**

Add two lines to `./language_server_dispatcher.cpp` to register the procedures
* Preprocessor: `#include {procedure_name}.h;`
* Constructor: `_register_procedure<lsp::{ProcedureName}>(String("{textDocument|workspace}/{ProcedureName}"));` 

## Open Todo List

This project is in early and active development.  Below are a set of items required to make it more complete:

- [ ] Automated Testing
- [ ] Better handling of JSON-RPC "Header Part" in Requests and Responses
- [ ] Increased Error Handling Throughout
- [ ] Standardized Error Responses
- [ ] Create an example provider
