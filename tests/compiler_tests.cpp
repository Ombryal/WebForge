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

int main() {
    testLexerBasicPage();
    testLexerButtonProgram();
    testParserBasicPage();
    testParserButtonProgram();
    testCodegenBasicPage();
    testCodegenButtonProgram();
    testCodegenEscapesSpecialCharacters();

    std::cout << "All tests passed\n";
    return 0;
}
