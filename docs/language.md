# WebForge Language Notes

This document describes the current WebForge language surface. It grows as
the compiler grows, everything below is implemented and covered by tests
unless marked otherwise.

## page

Declares the generated HTML page title. Every `.wf` file starts with exactly
one `page` statement.

    page "Hello World"

Compiles to:

    <title>Hello World</title>

## Comments

A `//` starts a line comment. Everything from `//` to the end of the line is
ignored.

    page "Hello" // shown in the browser tab

## String literals

Strings are double-quoted, and support a small set of backslash escapes:

| Escape | Meaning |
|--------|---------|
| `\"`   | literal `"` |
| `\\`   | literal `\` |
| `\n`   | newline |
| `\t`   | tab |

    raw "<hr class=\"divider\">"

An unrecognized escape like `\z` is kept as-is (both the backslash and the
character), rather than silently dropping the backslash.

## Numbers

A bare sequence of digits, optionally with one decimal point (`2`, `3.5`),
is a number literal. Currently the only place a number is used is the
optional heading level below; there's no arithmetic or numeric styling yet.

## Leaf statements

These each compile to a single element and take one or two string arguments.

### text

    text "Hello, world!"

Compiles to:

    <p>Hello, world!</p>

### heading

    heading "Welcome"
    heading "Section title" 2

Compiles to:

    <h1>Welcome</h1>
    <h2>Section title</h2>

The optional number is the heading level, clamped to 1–6. Omitting it
defaults to 1.

### image

    image "cat.png" "A cat"

Arguments are source and alt text. Compiles to:

    <img src="cat.png" alt="A cat">

### link

    link "Docs" "https://example.com"
    link "Docs" "https://example.com" "_blank"

Arguments are label, href, and an optional target. Compiles to:

    <a href="https://example.com">Docs</a>
    <a href="https://example.com" target="_blank" rel="noopener noreferrer">Docs</a>

The `rel="noopener noreferrer"` is added automatically whenever a target is
given.

## list

A block of items, one `item "..."` per line.

    list {
        item "One"
        item "Two"
    }

Compiles to:

    <ul>
      <li>One</li>
      <li>Two</li>
    </ul>

Add `ordered` before the block for a numbered list instead:

    list ordered {
        item "One"
        item "Two"
    }

Compiles to `<ol>` instead of `<ul>`, with the same `<li>` items.

## button

A label plus one or more event handlers.

    button "Click me" {
        on click {
            alert("Hello from WebForge!")
        }
        on hover {
            alert("You're hovering!")
        }
    }

`click` and `hover` are the two supported events (`hover` compiles to
`onmouseover`), and `alert(...)` the only supported action. A button can
have both at once, each rendered as its own attribute.

## container

Groups other statements together.

    container {
        heading "Welcome"
        text "Some body copy"
    }

Compiles to a `<div>` wrapping its children. A container can hold `text`,
`heading`, `image`, `link`, `button`, `list`, `raw`, and another
`container`, to any depth:

    container {
        heading "Outer"
        container {
            text "Inner"
        }
    }

Compiles to:

    <div>
      <h1>Outer</h1>
      <div>
        <p>Inner</p>
      </div>
    </div>

## Inline styling

Any of `text`, `heading`, `image`, `link`, `button`, `list`, or `container`
can carry a trailing block of `"property" "value"` pairs, rendered as an
inline `style` attribute.

    text "Styled" {
        "color" "blue"
        "font-size" "20px"
    }

Compiles to:

    <p style="color:blue; font-size:20px;">Styled</p>

For `button`, `list`, and `container` — which already use `{ ... }` for
event handlers, items, or children — style pairs can appear alongside those
entries in any order:

    container {
        "background" "gray"
        heading "Welcome"
    }

Property names and values are plain strings — there's no validation that
they're real CSS yet, so a typo like `"colr"` will compile silently and just
have no visual effect in the browser.

Two property names are special-cased: `"id"` and `"class"` are pulled out of
the style block and rendered as their own `id`/`class` attributes instead of
going into the inline `style`.

    text "Styled" {
        "id" "intro"
        "class" "lead"
        "color" "blue"
    }

Compiles to:

    <p id="intro" class="lead" style="color:blue;">Styled</p>

If `"class"` appears more than once, the values are space-joined into one
`class` attribute. If `"id"` appears more than once, only the first is used.

## stylesheet

A page-level statement linking an external CSS file. WebForge doesn't
generate CSS files itself yet — this just references one you maintain by
hand, which is what makes `class` above actually useful.

    stylesheet "styles.css"

Compiles to, inside `<head>`:

    <link rel="stylesheet" href="styles.css">

## meta

A page-level statement for an HTML `<meta>` tag, e.g. for SEO.

    meta "description" "A page built with WebForge."

Compiles to, inside `<head>`:

    <meta name="description" content="A page built with WebForge.">

`stylesheet` and `meta` are page-level only — neither is valid inside a
`container`.

## favicon

A page-level statement for the site's favicon.

    favicon "icon.ico"

Compiles to, inside `<head>`:

    <link rel="icon" href="icon.ico">

Page-level only, same as `stylesheet` and `meta`.

## raw — unescaped HTML escape hatch

    raw "<hr>"

Compiles to exactly:

    <hr>

**This is the one statement that skips HTML escaping entirely.** Every
other statement in WebForge escapes what you give it, so `text "<b>hi</b>"`
prints the literal characters `<b>hi</b>` rather than bolding anything.
`raw` does the opposite on purpose — whatever string you pass appears
verbatim in the output. That makes it easy to inject broken markup or, if
the string ever comes from something other than a hand-written literal
(user input, a file, a network response), a genuine XSS vulnerability. Only
use `raw` with content you wrote and trust; never with anything a user
could influence.

`raw` is valid both at page level and inside a `container`, since it's
inline content like `text` rather than head metadata like `stylesheet`.

## Planned next additions

Not implemented yet:

- WebForge-generated CSS (stylesheets are hand-written and only linked, for now)
- Variables, conditionals, and loops (numbers exist as a literal now, but
  there's no arithmetic, comparisons, or control flow yet)
- Multi-page projects and routing
