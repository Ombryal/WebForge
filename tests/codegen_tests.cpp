#include <cassert>
#include <cstddef>
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

void testCodegenStylesheetAndMeta() {
    std::string source =
        "page \"Hello\"\n"
        "\n"
        "stylesheet \"styles.css\"\n"
        "meta \"description\" \"A test page.\"\n"
        "text \"Body copy\"\n";

    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    ast::Page page = parser.parse();

    codegen::HtmlGenerator generator(page);
    std::string html = generator.generate();

    std::size_t headStart = html.find("<head>");
    std::size_t headEnd = html.find("</head>");
    std::size_t bodyStart = html.find("<body>");

    assert(headStart != std::string::npos);
    assert(headEnd != std::string::npos);
    assert(bodyStart != std::string::npos);

    std::size_t stylesheetPos = html.find(
        "<link rel=\"stylesheet\" href=\"styles.css\">"
    );
    std::size_t metaPos = html.find(
        "<meta name=\"description\" content=\"A test page.\">"
    );

    assert(stylesheetPos != std::string::npos);
    assert(metaPos != std::string::npos);

    // Both belong inside <head>, not <body>.
    assert(stylesheetPos > headStart && stylesheetPos < headEnd);
    assert(metaPos > headStart && metaPos < headEnd);

    assert(html.find("<p>Body copy</p>") > bodyStart);
}

void testCodegenIdAndClassAttributes() {
    std::string source =
        "page \"Hello\"\n"
        "\n"
        "text \"Styled\" {\n"
        "    \"id\" \"intro\"\n"
        "    \"class\" \"lead\"\n"
        "    \"color\" \"blue\"\n"
        "}\n";

    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    ast::Page page = parser.parse();

    codegen::HtmlGenerator generator(page);
    std::string html = generator.generate();

    assert(html.find("id=\"intro\"") != std::string::npos);
    assert(html.find("class=\"lead\"") != std::string::npos);
    assert(html.find("style=\"color:blue;\"") != std::string::npos);

    // id/class must not leak into the inline style attribute.
    assert(html.find("id:intro") == std::string::npos);
    assert(html.find("class:lead") == std::string::npos);
}

void testCodegenNestedContainers() {
    std::string source =
        "page \"Hello\"\n"
        "\n"
        "container {\n"
        "    heading \"Outer\"\n"
        "    container {\n"
        "        text \"Inner\"\n"
        "    }\n"
        "}\n";

    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    ast::Page page = parser.parse();

    codegen::HtmlGenerator generator(page);
    std::string html = generator.generate();

    std::size_t outerDivPos = html.find("<div>");
    assert(outerDivPos != std::string::npos);

    std::size_t innerDivPos = html.find("<div>", outerDivPos + 1);
    assert(innerDivPos != std::string::npos);

    std::size_t headingPos = html.find("<h1>Outer</h1>");
    std::size_t innerTextPos = html.find("<p>Inner</p>");

    assert(headingPos != std::string::npos);
    assert(innerTextPos != std::string::npos);

    // Heading belongs to the outer div, before the nested div starts.
    assert(headingPos > outerDivPos && headingPos < innerDivPos);

    // Inner text belongs inside the nested div.
    assert(innerTextPos > innerDivPos);

    // Two </div> closes for two levels of nesting.
    std::size_t firstClose = html.find("</div>");
    std::size_t secondClose = html.find("</div>", firstClose + 1);
    assert(firstClose != std::string::npos);
    assert(secondClose != std::string::npos);
}

void testCodegenClickAndHoverBothRender() {
    std::string source =
        "page \"Hello\"\n"
        "\n"
        "button \"Hover me\" {\n"
        "    on click {\n"
        "        alert(\"Clicked\")\n"
        "    }\n"
        "    on hover {\n"
        "        alert(\"Hovered\")\n"
        "    }\n"
        "}\n";

    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    ast::Page page = parser.parse();

    codegen::HtmlGenerator generator(page);
    std::string html = generator.generate();

    assert(html.find("onclick=\"alert(&#39;Clicked&#39;);\"") != std::string::npos);
    assert(html.find("onmouseover=\"alert(&#39;Hovered&#39;);\"") != std::string::npos);
}

void testCodegenFavicon() {
    std::string source =
        "page \"Hello\"\n"
        "\n"
        "favicon \"icon.ico\"\n";

    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    ast::Page page = parser.parse();

    codegen::HtmlGenerator generator(page);
    std::string html = generator.generate();

    std::size_t headStart = html.find("<head>");
    std::size_t headEnd = html.find("</head>");
    std::size_t faviconPos = html.find("<link rel=\"icon\" href=\"icon.ico\">");

    assert(faviconPos != std::string::npos);
    assert(faviconPos > headStart && faviconPos < headEnd);
}

void testCodegenRawHtmlIsUnescaped() {
    std::string source =
        "page \"Hello\"\n"
        "\n"
        "raw \"<hr class=\\\"divider\\\">\"\n";

    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    ast::Page page = parser.parse();

    codegen::HtmlGenerator generator(page);
    std::string html = generator.generate();

    // The whole point of `raw` is that it comes through literally, not
    // HTML-escaped like every other statement's content does.
    assert(html.find("<hr class=\"divider\">") != std::string::npos);
    assert(html.find("&lt;hr") == std::string::npos);
}
