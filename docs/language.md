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

## Leaf statements

These each compile to a single element and take one or two string arguments.

### text

    text "Hello, world!"

Compiles to:

    <p>Hello, world!</p>

### heading

    heading "Welcome"

Compiles to:

    <h1>Welcome</h1>

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

## button

A label plus one or more event handlers.

    button "Click me" {
        on click {
            alert("Hello from WebForge!")
        }
    }

`on click` is currently the only supported event, and `alert(...)` the only
supported action. Compiles to a `<button>` with an inline `onclick`.

## container

Groups other statements together.

    container {
        heading "Welcome"
        text "Some body copy"
    }

Compiles to a `<div>` wrapping its children. A container can hold `text`,
`heading`, `image`, `link`, `button`, `list`, and another `container`, to
any depth:

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

## Planned next additions

Not implemented yet:

- WebForge-generated CSS (stylesheets are hand-written and only linked, for now)
- Variables, conditionals, and loops
- Multi-page projects and routing
