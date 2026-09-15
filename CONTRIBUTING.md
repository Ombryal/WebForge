# Contributing to WebForge

Thanks for taking a look at WebForge. It's a young project and the
language is still taking shape, so process here is intentionally simple.

## Getting set up

You need a C++20 compiler and CMake 3.22+. See the [README](README.md)
for build and test commands.

## Branching

Branches are created from `dev`, not `main`. Prefixes:

- `feature/` — new language features or tooling
- `fix/` — bug fixes
- `refactor/` — internal restructuring with no behavior change
- `test/` — test-only changes
- `docs/` — documentation-only changes
- `ci/` — CI/workflow changes
- `chore/` — everything else (dependency bumps, config tweaks, etc.)

A branch can bundle a few small, related changes (e.g. two or three small
statements added together) rather than needing one branch per feature,
but keep each branch to a single coherent theme.

## Commits

Commit messages follow [Conventional Commits](https://www.conventionalcommits.org/):
a `type(scope): short summary` title, with `feat`, `fix`, `refactor`,
`test`, `docs`, `ci`, or `chore` as the type. A short body explaining the
change is welcome but not required.

Examples from the project's own history:

```
feat(parser): implement heading, image, and link statements
test: cover inline styling and link target
docs: document nested container support
```

## Adding a language feature

Most new statements touch the same four places, in this order:

1. **Lexer** — add any new keyword to `TokenType` (`Token.h`) and the
   `KEYWORDS` map (`Keywords.h`).
2. **AST** — add the new node to `Ast.h`, and to `Statement` and/or
   `ContainerChild` depending on whether it should be usable inside a
   `container`.
3. **Parser** — add a `parse*` method (in `ParserStatements.cpp`) and wire
   it into `parseStatement`'s dispatch (and `parseContainerChild`'s, if
   applicable) in `Parser.cpp`.
4. **Codegen** — add a `generate*` method in `CodeGen.cpp` and wire it into
   `generateStatement` (and `generateContainerChild`/`generateHeadExtra`,
   if applicable).

Then:

5. Add tests in the matching file under `tests/` (`lexer_tests.cpp`,
   `parser_tests.cpp`, `codegen_tests.cpp`) and register them in
   `compiler_tests.cpp`'s `main()`.
6. Update [`docs/language.md`](docs/language.md) with the new syntax and
   its HTML output.

## Code style

- Escape untrusted output by default. `HtmlGenerator::escapeHtml` handles
  HTML-context escaping; don't bypass it unless you're deliberately adding
  something like `raw` (and if you are, document the risk clearly).
- Keep files from growing unbounded. As a rough guide, once a single file
  crosses ~800–1,000 lines, consider splitting it by concern (the way
  `Parser.cpp`/`ParserStatements.cpp` and the `tests/*.cpp` files are
  already split) rather than continuing to grow it.
- Match the existing formatting and naming in the file you're editing over
  introducing a new style.

## Reporting bugs and proposing features

Open an issue. For anything security-related, see
[`SECURITY.md`](SECURITY.md) instead of a public issue.
