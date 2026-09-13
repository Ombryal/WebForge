#pragma once

#include <string>
#include <vector>

#include "webforge/lexer/Token.h"

namespace webforge {

class Lexer {
public:
    explicit Lexer(std::string source);

    std::vector<Token> tokenize();

private:
    bool isAtEnd() const;
    char advance();
    char peek() const;

    void skipWhitespace();

    Token scanToken();
    Token makeToken(TokenType type, std::string value) const;

    Token scanString();
    Token scanIdentifierOrKeyword();
    Token scanSingleCharToken(TokenType type);

    static bool isIdentifierStart(char c);
    static bool isIdentifierPart(char c);

    std::string source_;
    std::size_t position_ = 0;
    int line_ = 1;
    int column_ = 1;

    int tokenStartLine_ = 1;
    int tokenStartColumn_ = 1;
};

} // namespace webforge
