# WebForge Language Notes

This document describes the current WebForge language surface. It grows as
the compiler grows — everything below is implemented and covered by tests
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

Arguments are label and href. Compiles to:

    <a href="https://example.com">Docs</a>

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
`heading`, `image`, `link`, `button`, and `list` — **not** another
`container`. Nested containers aren't supported yet; that needs a recursive
AST design that hasn't been introduced.

## Planned next additions

Not implemented yet:

- Inline or block-level styling (colors, spacing, fonts)
- Nested containers
- Variables, conditionals, and loops
- Multi-page projects and routing
