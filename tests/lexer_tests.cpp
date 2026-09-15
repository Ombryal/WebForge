#include <cassert>
#include <string>
#include <vector>

#include "TestCases.h"
#include "webforge/lexer/Lexer.h"

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

void testLexerStringEscapeSequences() {
    // Source text: raw "<hr class=\"divider\">\nline two\\end"
    std::string source =
        "raw \"<hr class=\\\"divider\\\">\\nline two\\\\end\"";

    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();

    assert(tokens.size() == 3);
    assert(tokens[0].type == TokenType::KeywordRaw);
    assert(tokens[1].type == TokenType::String);
    assert(tokens[1].value == "<hr class=\"divider\">\nline two\\end");
    assert(tokens[2].type == TokenType::Eof);
}

void testLexerNumericLiteral() {
    std::string source = "heading \"Title\" 2";

    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();

    assert(tokens.size() == 4);
    assert(tokens[0].type == TokenType::KeywordHeading);
    assert(tokens[1].type == TokenType::String);
    assert(tokens[2].type == TokenType::Number);
    assert(tokens[2].value == "2");
    assert(tokens[3].type == TokenType::Eof);
}
