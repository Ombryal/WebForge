#include <cassert>
#include <string>

#include "TestCases.h"
#include "webforge/codegen/CodeGen.h"
#include "webforge/lexer/Lexer.h"
#include "webforge/parser/Parser.h"

using namespace webforge;

void testCodegenBasicPage() {
    std::string source =
        "page \"Hello World\"\n"
        "\n"
        "text \"Hello, world!\"\n";

    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    ast::Page page = parser.parse();

    codegen::HtmlGenerator generator(page);
    std::string html = generator.generate();

    assert(html.find("<title>Hello World</title>") != std::string::npos);
    assert(html.find("<p>Hello, world!</p>") != std::string::npos);
}

void testCodegenButtonProgram() {
    std::string source =
        "page \"Hello\"\n"
        "\n"
        "button \"Click me\" {\n"
        "    on click {\n"
        "        alert(\"Hi\")\n"
        "    }\n"
        "}\n";

    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    ast::Page page = parser.parse();

    codegen::HtmlGenerator generator(page);
    std::string html = generator.generate();

    assert(html.find(">Click me</button>") != std::string::npos);
    assert(html.find("onclick=\"alert(&#39;Hi&#39;);\"") != std::string::npos);
}

void testCodegenEscapesSpecialCharacters() {
    std::string source =
        "page \"A & B\"\n"
        "\n"
        "text \"<script>alert('x')</script>\"\n";

    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    ast::Page page = parser.parse();

    codegen::HtmlGenerator generator(page);
    std::string html = generator.generate();

    assert(html.find("<title>A &amp; B</title>") != std::string::npos);
    assert(html.find("<script>alert") == std::string::npos);
    assert(html.find("&lt;script&gt;") != std::string::npos);
}

void testCodegenBasicElements() {
    std::string source =
        "page \"Hello\"\n"
        "\n"
        "heading \"Welcome\"\n"
        "image \"cat.png\" \"A cat\"\n"
        "link \"Docs\" \"https://example.com\"\n";

    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    ast::Page page = parser.parse();

    codegen::HtmlGenerator generator(page);
    std::string html = generator.generate();

    assert(html.find("<h1>Welcome</h1>") != std::string::npos);
    assert(html.find("<img src=\"cat.png\" alt=\"A cat\">") != std::string::npos);
    assert(html.find("<a href=\"https://example.com\">Docs</a>") != std::string::npos);
}

void testCodegenListAndContainer() {
    std::string source =
        "page \"Hello\"\n"
        "\n"
        "container {\n"
        "    heading \"Welcome\"\n"
        "    list {\n"
        "        item \"One\"\n"
        "        item \"Two\"\n"
        "    }\n"
        "}\n";

    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    ast::Page page = parser.parse();

    codegen::HtmlGenerator generator(page);
    std::string html = generator.generate();

    assert(html.find("<div>") != std::string::npos);
    assert(html.find("<h1>Welcome</h1>") != std::string::npos);
    assert(html.find("<ul>") != std::string::npos);
    assert(html.find("<li>One</li>") != std::string::npos);
    assert(html.find("<li>Two</li>") != std::string::npos);
    assert(html.find("</div>") != std::string::npos);
}

void testCodegenInlineStyle() {
    std::string source =
        "page \"Hello\"\n"
        "\n"
        "text \"Styled\" {\n"
        "    \"color\" \"blue\"\n"
        "}\n";

    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    ast::Page page = parser.parse();

    codegen::HtmlGenerator generator(page);
    std::string html = generator.generate();

    assert(html.find("style=\"color:blue;\"") != std::string::npos);
}

void testCodegenLinkTargetAndRel() {
    std::string source =
        "page \"Hello\"\n"
        "\n"
        "link \"Docs\" \"https://example.com\" \"_blank\"\n";

    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    ast::Page page = parser.parse();

    codegen::HtmlGenerator generator(page);
    std::string html = generator.generate();

    assert(html.find("target=\"_blank\"") != std::string::npos);
    assert(html.find("rel=\"noopener noreferrer\"") != std::string::npos);
}
