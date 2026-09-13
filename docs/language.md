# WebForge Language Notes

This document is an early placeholder for the WebForge language design.

It is not final. The goal is to define just enough language surface to build
the first working compiler milestone.

## Current milestone

The first compiler milestone is static HTML generation.

Example:

    page "Hello World"

    text "Hello, world!"

Expected compilation output:

    index.html

No CSS or JavaScript compilation is required for this first milestone.

## Initial language ideas

### page

Declares the generated HTML page title.

Example:

    page "Hello World"

This should eventually map to:

    <title>Hello World</title>

### text

Emits a simple text element.

Example:

    text "Hello, world!"

This should eventually map to something like:

    <p>Hello, world!</p>

## Planned next milestone

Button support with a simple event handler:

    button "Click me" {
        on click {
            alert("Hello from WebForge!")
        }
    }

This is not implemented yet.
