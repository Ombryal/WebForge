# Security Policy

## Scope

WebForge is a local command-line compiler: you run it against a `.wf` file
on your own machine (or CI runner) and it writes an `.html` file to disk.
It isn't a hosted service and doesn't process anyone else's input over a
network, so the realistic attack surface is narrow — mainly the compiler
itself misbehaving on malformed or adversarial `.wf` source, and the HTML
it generates being safe to serve.

One thing to know that isn't a bug: the `raw` statement is a deliberate
escape hatch that emits its string completely unescaped, unlike every
other statement. That's documented behavior, not a vulnerability — see
[`docs/language.md`](docs/language.md#raw--unescaped-html-escape-hatch) for
what it does and why it's risky to use with untrusted input. If you find a
way for *other* statements (`text`, `heading`, attribute values, etc.) to
produce unescaped or otherwise unsafe output, that's a real bug and worth
reporting.

## Supported versions

There are no tagged releases yet — `dev` is the only actively maintained
line. Fixes land there first.

## Reporting a vulnerability

If you find something that looks like a genuine security issue (e.g. the
compiler generating unescaped output somewhere it shouldn't, or a crash on
untrusted input that looks exploitable), please use GitHub's private
["Report a vulnerability"](../../security/advisories/new) flow on this
repository instead of opening a public issue, so it can be looked at before
details are public.

For anything else (general bugs, feature requests, questions), a regular
GitHub issue is the right place — see [`CONTRIBUTING.md`](CONTRIBUTING.md).

This is a personal project maintained in spare time, so response times are
best-effort rather than guaranteed.
