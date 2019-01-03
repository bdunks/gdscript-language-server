//
// This file contains structs based on the LSP specification at
// https://github.com/Microsoft/language-server-protocol/blob/master/protocol.md
//
// The initial structure definitions of this file were taken from the llvm clangd lsp
// implementation, and as such some comments may be the same.
//
// This is not meant to be a complete implementation, new interfaces are added
// when they're needed.
//
//===----------------------------------------------------------------------===//

#ifndef LSP_PROTOCOL_H
#define LSP_PROTOCOL_H

#include "core/variant.h"

namespace lsp {

using DocumentUri = String;

struct TextDocumentIdentifier {
	/// The text document's URI.
	DocumentUri uri;
};

struct Position {
	/// Line position in a document (zero-based).
	int line = 0;

	/// Character offset on a line in a document (zero-based).
	/// WARNING: this is in UTF-16 codepoints, not bytes or characters!
	/// Use the functions in SourceCode.h to construct/interpret Positions.
	int character = 0;

	friend bool operator==(const Position &LHS, const Position &RHS) {
		return (LHS.line == RHS.line) && (LHS.character == RHS.character);
	}

	friend bool operator!=(const Position &LHS, const Position &RHS) {
		return !(LHS == RHS);
	}

	friend bool operator<(const Position &LHS, const Position &RHS) {
		return (LHS.line < RHS.line) ||
		       ((LHS.line == RHS.line) && (LHS.character < RHS.character));
	}

	friend bool operator<=(const Position &LHS, const Position &RHS) {
		return (LHS.line <= RHS.line) ||
		       ((LHS.line == RHS.line) && (LHS.character <= RHS.character));
	}
};

struct Range {
	/// The range's start position.
	Position start;

	/// The range's end position.
	Position end;

	friend bool operator==(const Range &LHS, const Range &RHS) {
		return (LHS.start == RHS.start) && (LHS.end == RHS.end);
	}

	friend bool operator!=(const Range &LHS, const Range &RHS) {
		return !(LHS == RHS);
	}

	friend bool operator<(const Range &LHS, const Range &RHS) {
		return (LHS.start < RHS.start) ||
		       ((LHS.start == RHS.start) && (LHS.end < RHS.end));
	}

	bool contains(Position Pos) const { return start <= Pos && Pos < end; }
};

struct Location {
	/// The text document's URI.
	DocumentUri uri;
	Range range;

	friend bool operator==(const Location &LHS, const Location &RHS) {
		return LHS.uri == RHS.uri && LHS.range == RHS.range;
	}

	friend bool operator!=(const Location &LHS, const Location &RHS) {
		return !(LHS == RHS);
	}
};

struct TextEdit {
	/// The range of the text document to be manipulated. To insert
	/// text into a document create a range where start === end.
	Range range;

	/// The string to be inserted. For delete operations use an
	/// empty string.
	String newText;
};

struct TextDocumentItem {
	/// The text document's URI.
	DocumentUri uri;

	/// The text document's language identifier.
	String languageId;

	/// The version number of this document (it will strictly increase after each
	int version = 0;

	/// The content of the opened text document.
	String text;
};

enum class TraceLevel {
	Off = 0,
	Messages = 1,
	Verbose = 2,
};

struct NoParams {
};

using ShutdownParams = NoParams;
using ExitParams = NoParams;

/// Defines how the host (editor) should sync document changes to the language
/// server.
enum class TextDocumentSyncKind {
	/// Documents should not be synced at all.
	None = 0,

	/// Documents are synced by always sending the full content of the document.
	Full = 1,

	/// Documents are synced by sending the full content on open.  After that
	/// only incremental updates to the document are send.
	Incremental = 2,
};

struct CompletionItemClientCapabilities {
	/// Client supports snippets as insert text.
	bool snippetSupport = false;
	/// Client supports commit characters on a completion item.
	bool commitCharacterSupport = false;
};

struct CompletionClientCapabilities {
	/// Whether completion supports dynamic registration.
	bool dynamicRegistration = false;
	/// The client supports the following `CompletionItem` specific capabilities.
	CompletionItemClientCapabilities completionItem;

	/// The client supports to send additional context information for a
	/// `textDocument/completion` request.
	bool contextSupport = false;
};

/// A symbol kind.
enum class SymbolKind {
	File = 1,
	Module = 2,
	Namespace = 3,
	Package = 4,
	Class = 5,
	Method = 6,
	Property = 7,
	Field = 8,
	Constructor = 9,
	Enum = 10,
	Interface = 11,
	Function = 12,
	Variable = 13,
	Constant = 14,
	String = 15,
	Number = 16,
	Boolean = 17,
	Array = 18,
	Object = 19,
	Key = 20,
	Null = 21,
	EnumMember = 22,
	Struct = 23,
	Event = 24,
	Operator = 25,
	TypeParameter = 26
};

struct SymbolKindCapabilities {
	/// The SymbolKinds that the client supports. If not set, the client only
	/// supports <= SymbolKind::Array and will not fall back to a valid default
	/// value.
	Array valueSet;
};

struct WorkspaceSymbolCapabilities {
	/// Capabilities SymbolKind.
	SymbolKindCapabilities symbolKind;
};

// FIXME: most of the capabilities are missing from this struct. Only the ones
// used by clangd have been ported over
struct WorkspaceClientCapabilities {
	/// Capabilities specific to `workspace/symbol`.
	WorkspaceSymbolCapabilities symbol;
};

// FIXME: most of the capabilities are missing from this struct. Only the ones
// used by clangd  have been ported over
struct TextDocumentClientCapabilities {
	/// Capabilities specific to the `textDocument/completion`
	CompletionClientCapabilities completion;
};

struct ClientCapabilities {
	// Workspace specific client capabilities.
	WorkspaceClientCapabilities workspace;

	// Text document specific client capabilities.
	TextDocumentClientCapabilities textDocument;
};

struct InitializeParams {
	/// The process Id of the parent process that started
	/// the server. Is null if the process has not been started by another
	/// process. If the parent process is not alive then the server should exit
	/// (see exit notification) its process.
	int processId;

	/// The rootPath of the workspace. Is null
	/// if no folder is open.
	///
	/// @deprecated in favour of rootUri.
	String rootPath;

	/// The rootUri of the workspace. Is null if no
	/// folder is open. If both `rootPath` and `rootUri` are set
	/// `rootUri` wins.
	DocumentUri rootUri;

	// User provided initialization options.
	Variant initializationOptions;

	/// The capabilities provided by the client (editor or tool)
	ClientCapabilities capabilities;

	/// The initial trace setting. If omitted trace is disabled ('off').
	TraceLevel trace;

	// We use this predefined struct because it is easier to use
	// than the protocol specified type of 'any'.
};

struct DidOpenTextDocumentParams {
	/// The document that was opened.
	TextDocumentItem textDocument;
};

struct DidCloseTextDocumentParams {
	/// The document that was closed.
	TextDocumentIdentifier textDocument;
};

struct TextDocumentContentChangeEvent {
	/// The range of the document that changed.
	Range range;

	/// The length of the range that got replaced.
	int rangeLength;

	/// The new text of the range/document.
	String text;
};

struct DidChangeTextDocumentParams {
	/// The document that did change. The version number points
	/// to the version after all provided content changes have
	/// been applied.
	TextDocumentIdentifier textDocument;

	/// The actual content changes.
	Variant contentChanges;
};

enum class FileChangeType {
	/// The file got created.
	Created = 1,
	/// The file got changed.
	Changed = 2,
	/// The file got deleted.
	Deleted = 3
};

struct FileEvent {
	/// The file's URI.
	DocumentUri uri;
	/// The change type.
	FileChangeType type = FileChangeType::Created;
};

struct DidChangeWatchedFilesParams {
	/// The actual file events.
	Variant changes;
};

struct DidChangeConfigurationParams {
	// We use this predefined struct because it is easier to use
	// than the protocol specified type of 'any'.
	Variant settings;
};

struct FormattingOptions {
	/// Size of a tab in spaces.
	int tabSize = 0;

	/// Prefer spaces over tabs.
	bool insertSpaces = false;
};

struct DocumentRangeFormattingParams {
	/// The document to format.
	TextDocumentIdentifier textDocument;

	/// The range to format
	Range range;

	/// The format options
	FormattingOptions options;
};

struct DocumentOnTypeFormattingParams {
	/// The document to format.
	TextDocumentIdentifier textDocument;

	/// The position at which this request was sent.
	Position position;

	/// The character that has been typed.
	String ch;

	/// The format options.
	FormattingOptions options;
};

struct DocumentFormattingParams {
	/// The document to format.
	TextDocumentIdentifier textDocument;

	/// The format options
	FormattingOptions options;
};

struct DocumentSymbolParams {
	// The text document to find symbols in.
	TextDocumentIdentifier textDocument;
};

struct Diagnostic {
	/// The range at which the message applies.
	Range range;

	/// The diagnostic's severity. Can be omitted. If omitted it is up to the
	/// client to interpret diagnostics as error, warning, info or hint.
	int severity = 0;

	/// The diagnostic's code. Can be omitted.
	String code;

	/// A human-readable string describing the source of this
	/// diagnostic, e.g. 'typescript' or 'super lint'.
	String source;

	/// The diagnostic's message.
	String message;
};


struct CodeActionContext {
	/// An array of diagnostics.
	Array diagnostics;
};

struct CodeActionParams {
	/// The document in which the command was invoked.
	TextDocumentIdentifier textDocument;

	/// The range for which the command was invoked.
	Range range;

	/// Context carrying additional information.
	CodeActionContext context;
};

struct WorkspaceEdit {
	/// Holds changes to existing resources.
	/// map<String,Vector<TextEdit> >
	Dictionary changes;

	/// Note: "documentChanges" is not currently used because currently there is
	/// no support for versioned edits.
};

struct ExecuteCommandParams {
	String command;

	// Arguments
	Variant arguments;
};

struct Command : public ExecuteCommandParams {
	String title;
};

/// Represents information about programming constructs like variables, classes,
/// interfaces etc.
struct SymbolInformation {
	/// The name of this symbol.
	String name;

	/// The kind of this symbol.
	SymbolKind kind;

	/// The location of this symbol.
	Location location;

	/// The name of the symbol containing this symbol.
	String containerName;
};

/// The parameters of a Workspace Symbol Request.
struct WorkspaceSymbolParams {
	/// A non-empty query string
	String query;
};

struct ApplyWorkspaceEditParams {
	WorkspaceEdit edit;
};

struct TextDocumentPositionParams {
	/// The text document.
	TextDocumentIdentifier textDocument;

	/// The position inside the text document.
	Position position;
};

enum class MarkupKind {
	PlainText,
	Markdown,
};

struct MarkupContent {
	MarkupKind kind = MarkupKind::PlainText;
	String value;
};

struct Hover {
	/// The hover's content
	MarkupContent contents;

	/// An optional range is a range inside a text document
	/// that is used to visualize a hover, e.g. by changing the background color.
	Range range;
};

/// The kind of a completion entry.
enum class CompletionItemKind {
	Missing = 0,
	Text = 1,
	Method = 2,
	Function = 3,
	Constructor = 4,
	Field = 5,
	Variable = 6,
	Class = 7,
	Interface = 8,
	Module = 9,
	Property = 10,
	Unit = 11,
	Value = 12,
	Enum = 13,
	Keyword = 14,
	Snippet = 15,
	Color = 16,
	File = 17,
	Reference = 18,
};

/// Defines whether the insert text in a completion item should be interpreted
/// as plain text or a snippet.
enum class InsertTextFormat {
	Missing = 0,
	/// The primary text to be inserted is treated as a plain string.
	PlainText = 1,
	/// The primary text to be inserted is treated as a snippet.
	///
	/// A snippet can define tab stops and placeholders with `$1`, `$2`
	/// and `${3:foo}`. `$0` defines the final tab stop, it defaults to the end
	/// of the snippet. Placeholders with equal identifiers are linked, that is
	/// typing in one will update others too.
	///
	/// See also:
	/// https//github.com/Microsoft/vscode/blob/master/src/vs/editor/contrib/snippet/common/snippet.md
	Snippet = 2,
};

struct CompletionItem {
	/// The label of this completion item. By default also the text that is
	/// inserted when selecting this completion.
	String label;

	/// The kind of this completion item. Based of the kind an icon is chosen by
	/// the editor.
	CompletionItemKind kind = CompletionItemKind::Missing;

	/// A human-readable string with additional information about this item, like
	/// type or symbol information.
	String detail;

	/// A human-readable string that represents a doc-comment.
	String documentation;

	/// A string that should be used when comparing this item with other items.
	/// When `falsy` the label is used.
	String sortText;

	/// A string that should be used when filtering a set of completion items.
	/// When `falsy` the label is used.
	String filterText;

	/// A string that should be inserted to a document when selecting this
	/// completion. When `falsy` the label is used.
	String insertText;

	/// The format of the insert text. The format applies to both the `insertText`
	/// property and the `newText` property of a provided `textEdit`.
	InsertTextFormat insertTextFormat = InsertTextFormat::Missing;

	/// An edit which is applied to a document when selecting this completion.
	/// When an edit is provided `insertText` is ignored.
	///
	/// Note: The range of the edit must be a single line range and it must
	/// contain the position at which completion has been requested.
	TextEdit textEdit;

	/// An optional array of additional text edits that are applied when selecting
	/// this completion. Edits must not overlap with the main edit nor with
	/// themselves.
	// Vector<TextEdit>
	Array additionalTextEdits;

	// TODO(krasimir): The following optional fields defined by the language
	// server protocol are unsupported:
	//
	// data?: any - A data entry field that is preserved on a completion item
	//              between a completion and a completion resolve request.
};

/// Represents a collection of completion items to be presented in the editor.
struct CompletionList {
	/// The list is not complete. Further typing should result in recomputing the
	/// list.
	bool isIncomplete = false;

	/// The completion items.
	// Vector<CompletionItem>
	Array items;
};

/// A single parameter of a particular signature.
struct ParameterInformation {

	/// The label of this parameter. Mandatory.
	String label;

	/// The documentation of this parameter. Optional.
	String documentation;
};

/// Represents the signature of something callable.
struct SignatureInformation {

	/// The label of this signature. Mandatory.
	String label;

	/// The documentation of this signature. Optional.
	String documentation;

	/// The parameters of this signature.
	// std::vector<ParameterInformation> 
	Array parameters;
};

/// Represents the signature of a callable.
struct SignatureHelp {

	/// The resulting signatures.
	// std::vector<SignatureInformation>
	Variant signatures;

	/// The active signature.
	int activeSignature = 0;

	/// The active parameter of the active signature.
	int activeParameter = 0;
};

struct RenameParams {
	/// The document that was opened.
	TextDocumentIdentifier textDocument;

	/// The position at which this request was sent.
	Position position;

	/// The new name of the symbol.
	String newName;
};

enum class DocumentHighlightKind {
	Text = 1,
	Read = 2,
	Write = 3
};

/// A document highlight is a range inside a text document which deserves
/// special attention. Usually a document highlight is visualized by changing
/// the background color of its range.

struct DocumentHighlight {
	/// The range this highlight applies to.
	Range range;

	/// The highlight kind, default is DocumentHighlightKind.Text.
	DocumentHighlightKind kind = DocumentHighlightKind::Text;

	friend bool operator<(const DocumentHighlight &LHS,
	                      const DocumentHighlight &RHS) {
		int LHSKind = static_cast<int>(LHS.kind);
		int RHSKind = static_cast<int>(RHS.kind);
		return (LHS.range < RHS.range) || ((LHS.range == RHS.range) && (LHSKind <  RHSKind));
	}

	friend bool operator==(const DocumentHighlight &LHS,
	                       const DocumentHighlight &RHS) {
		return LHS.kind == RHS.kind && LHS.range == RHS.range;
	}
};

} // namespace lsp
#endif
