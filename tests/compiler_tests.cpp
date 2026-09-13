#include <cassert>
#include <iostream>
#include <string>
#include <variant>
#include <vector>

#include "webforge/codegen/CodeGen.h"
#include "webforge/lexer/Lexer.h"
#include "webforge/parser/Parser.h"

using namespace webforge;

void testLexerBasicPage() {
    std::string source =
        "page \"Hello World\"\n"
        "\n"
        "text \"Hello, world!\"\n";

    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();

    assert(tokens.size() == 5);

    assert(tokens[0].type == TokenType::KeywordPage);

    assert(tokens[1].type == TokenType::String);
    assert(tokens[1].value == "Hello World");

    assert(tokens[2].type == TokenType::KeywordText);

    assert(tokens[3].type == TokenType::String);
    assert(tokens[3].value == "Hello, world!");

    assert(tokens[4].type == TokenType::Eof);
}

void testLexerButtonProgram() {
    std::string source =
        "page \"Hello\"\n"
        "\n"
        "button \"Click me\" {\n"
        "    on click {\n"
        "        alert(\"Hi\")\n"
        "    }\n"
        "}\n";

    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();

    assert(tokens.size() == 15);

    assert(tokens[0].type == TokenType::KeywordPage);
    assert(tokens[1].type == TokenType::String);
    assert(tokens[1].value == "Hello");

    assert(tokens[2].type == TokenType::KeywordButton);
    assert(tokens[3].type == TokenType::String);
    assert(tokens[3].value == "Click me");

    assert(tokens[4].type == TokenType::LeftBrace);

    assert(tokens[5].type == TokenType::KeywordOn);
    assert(tokens[6].type == TokenType::KeywordClick);

    assert(tokens[7].type == TokenType::LeftBrace);

    assert(tokens[8].type == TokenType::KeywordAlert);
    assert(tokens[9].type == TokenType::LeftParen);

    assert(tokens[10].type == TokenType::String);
    assert(tokens[10].value == "Hi");

    assert(tokens[11].type == TokenType::RightParen);
    assert(tokens[12].type == TokenType::RightBrace);
    assert(tokens[13].type == TokenType::RightBrace);

    assert(tokens[14].type == TokenType::Eof);
}

void testParserBasicPage() {
    std::string source =
        "page \"Hello World\"\n"
        "\n"
        "text \"Hello, world!\"\n";

    Lexer lexer(source);
    Parser parser(lexer.tokenize());

    ast::Page page = parser.parse();

    assert(page.title == "Hello World");
    assert(page.statements.size() == 1);

    const ast::TextStatement* text =
        std::get_if<ast::TextStatement>(&page.statements[0]);

    assert(text != nullptr);
    assert(text->text == "Hello, world!");
}

void testParserButtonProgram() {
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

    assert(page.title == "Hello");
    assert(page.statements.size() == 1);

    const ast::ButtonStatement* button =
        std::get_if<ast::ButtonStatement>(&page.statements[0]);

    assert(button != nullptr);
    assert(button->label == "Click me");
    assert(button->handlers.size() == 1);

    const ast::EventHandler& handler = button->handlers[0];

    assert(handler.eventName == "click");
    assert(handler.actions.size() == 1);

    const ast::AlertAction* alert =
        std::get_if<ast::AlertAction>(&handler.actions[0]);

    assert(alert != nullptr);
    assert(alert->message == "Hi");
}

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

void testParserBasicElements() {
    std::string source =
        "page \"Hello\"\n"
        "\n"
        "heading \"Welcome\"\n"
        "image \"cat.png\" \"A cat\"\n"
        "link \"Docs\" \"https://example.com\"\n";

    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    ast::Page page = parser.parse();

    assert(page.statements.size() == 3);

    const ast::HeadingStatement* heading =
        std::get_if<ast::HeadingStatement>(&page.statements[0]);
    assert(heading != nullptr);
    assert(heading->text == "Welcome");

    const ast::ImageStatement* image =
        std::get_if<ast::ImageStatement>(&page.statements[1]);
    assert(image != nullptr);
    assert(image->src == "cat.png");
    assert(image->altText == "A cat");

    const ast::LinkStatement* link =
        std::get_if<ast::LinkStatement>(&page.statements[2]);
    assert(link != nullptr);
    assert(link->label == "Docs");
    assert(link->href == "https://example.com");
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

void testParserListStatement() {
    std::string source =
        "page \"Hello\"\n"
        "\n"
        "list {\n"
        "    item \"One\"\n"
        "    item \"Two\"\n"
        "}\n";

    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    ast::Page page = parser.parse();

    assert(page.statements.size() == 1);

    const ast::ListStatement* list =
        std::get_if<ast::ListStatement>(&page.statements[0]);

    assert(list != nullptr);
    assert(list->items.size() == 2);
    assert(list->items[0] == "One");
    assert(list->items[1] == "Two");
}

void testParserContainerStatement() {
    std::string source =
        "page \"Hello\"\n"
        "\n"
        "container {\n"
        "    heading \"Welcome\"\n"
        "    text \"Body copy\"\n"
        "}\n";

    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    ast::Page page = parser.parse();

    assert(page.statements.size() == 1);

    const ast::ContainerStatement* container =
        std::get_if<ast::ContainerStatement>(&page.statements[0]);

    assert(container != nullptr);
    assert(container->children.size() == 2);

    const ast::HeadingStatement* heading =
        std::get_if<ast::HeadingStatement>(&container->children[0]);
    assert(heading != nullptr);
    assert(heading->text == "Welcome");

    const ast::TextStatement* text =
        std::get_if<ast::TextStatement>(&container->children[1]);
    assert(text != nullptr);
    assert(text->text == "Body copy");
}

void testParserRejectsNestedContainer() {
    std::string source =
        "page \"Hello\"\n"
        "\n"
        "container {\n"
        "    container {\n"
        "        text \"Nested\"\n"
        "    }\n"
        "}\n";

    Lexer lexer(source);
    Parser parser(lexer.tokenize());

    bool threw = false;
    try {
        parser.parse();
    } catch (const ParseError&) {
        threw = true;
    }

    assert(threw);
}

void testLexerSkipsComments() {
    std::string source =
        "page \"Hello\" // page title\n"
        "\n"
        "// a comment on its own line\n"
        "text \"Hello, world!\"\n";

    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();

    assert(tokens.size() == 5);
    assert(tokens[0].type == TokenType::KeywordPage);
    assert(tokens[1].type == TokenType::String);
    assert(tokens[1].value == "Hello");
    assert(tokens[2].type == TokenType::KeywordText);
    assert(tokens[3].type == TokenType::String);
    assert(tokens[3].value == "Hello, world!");
    assert(tokens[4].type == TokenType::Eof);
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

void testParserStyleOnLeafStatement() {
    std::string source =
        "page \"Hello\"\n"
        "\n"
        "text \"Styled\" {\n"
        "    \"color\" \"blue\"\n"
        "    \"font-size\" \"20px\"\n"
        "}\n";

    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    ast::Page page = parser.parse();

    const ast::TextStatement* text =
        std::get_if<ast::TextStatement>(&page.statements[0]);

    assert(text != nullptr);
    assert(text->style.size() == 2);
    assert(text->style[0].name == "color");
    assert(text->style[0].value == "blue");
    assert(text->style[1].name == "font-size");
    assert(text->style[1].value == "20px");
}

void testParserStyleInsideButtonAndContainer() {
    std::string source =
        "page \"Hello\"\n"
        "\n"
        "container {\n"
        "    \"background\" \"gray\"\n"
        "    button \"Click me\" {\n"
        "        \"color\" \"white\"\n"
        "        on click {\n"
        "            alert(\"Hi\")\n"
        "        }\n"
        "    }\n"
        "}\n";

    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    ast::Page page = parser.parse();

    const ast::ContainerStatement* container =
        std::get_if<ast::ContainerStatement>(&page.statements[0]);

    assert(container != nullptr);
    assert(container->style.size() == 1);
    assert(container->style[0].name == "background");
    assert(container->children.size() == 1);

    const ast::ButtonStatement* button =
        std::get_if<ast::ButtonStatement>(&container->children[0]);

    assert(button != nullptr);
    assert(button->style.size() == 1);
    assert(button->style[0].name == "color");
    assert(button->handlers.size() == 1);
}

void testParserLinkTarget() {
    std::string source =
        "page \"Hello\"\n"
        "\n"
        "link \"Docs\" \"https://example.com\" \"_blank\"\n";

    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    ast::Page page = parser.parse();

    const ast::LinkStatement* link =
        std::get_if<ast::LinkStatement>(&page.statements[0]);

    assert(link != nullptr);
    assert(link->target == "_blank");
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

int main() {
    testLexerBasicPage();
    testLexerButtonProgram();
    testLexerSkipsComments();
    testParserBasicPage();
    testParserButtonProgram();
    testParserBasicElements();
    testParserListStatement();
    testParserContainerStatement();
    testParserRejectsNestedContainer();
    testParserStyleOnLeafStatement();
    testParserStyleInsideButtonAndContainer();
    testParserLinkTarget();
    testCodegenBasicPage();
    testCodegenButtonProgram();
    testCodegenEscapesSpecialCharacters();
    testCodegenBasicElements();
    testCodegenListAndContainer();
    testCodegenInlineStyle();
    testCodegenLinkTargetAndRel();

    std::cout << "All tests passed\n";
    return 0;
}
