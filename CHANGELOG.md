# Changelog

All notable changes to WebForge are documented here. Format loosely follows
[Keep a Changelog](https://keepachangelog.com/). There are no tagged
releases yet, so everything so far sits under Unreleased.

## [Unreleased]

### Added

- `page` statement compiling to the document `<title>`.
- `text`, `heading`, `image`, and `link` leaf statements.
- `list` statement (`item "..."` entries) compiling to `<ul>`/`<li>`.
- `button` with `on click` / `on hover` event handlers and `alert(...)`
  actions, compiling to `onclick` / `onmouseover`.
- `container` statement for grouping other statements into a `<div>`,
  including arbitrary nesting depth (containers inside containers).
- `//` line comments.
- Inline styling: a trailing `"property" "value"` block on most statements,
  compiled to an inline `style` attribute. `"id"` and `"class"` are
  special-cased into their own attributes instead of the inline style.
- Optional `target` argument on `link`, with automatic
  `rel="noopener noreferrer"` when set.
- `stylesheet`, `meta`, and `favicon` page-level statements, compiling to
  `<head>` content.
- `raw` escape hatch for emitting literal, unescaped HTML.
- HTML-escaping by default across all statements except `raw`.
- CLI: `webforge <input.wf> [output.html]`.
- Test suite covering the lexer, parser, and codegen layers.
- `CONTRIBUTING.md`, `SECURITY.md`, `CODE_OF_CONDUCT.md`, and
  `examples/hello.wf`.

### Changed

- CI now runs manually from the Actions tab (`workflow_dispatch`) instead
  of on every push or pull request.
- Split `Parser.cpp` into `Parser.cpp` (core dispatch and token-stream
  helpers) and `ParserStatements.cpp` (individual statement parsers), and
  split `compiler_tests.cpp` into per-stage test files, to keep individual
  files from growing unbounded.
