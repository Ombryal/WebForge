#include "webforge/lexer/Lexer.h"
#include "webforge/lexer/Keywords.h"

#include <cctype>
#include <stdexcept>

namespace webforge {

Lexer::Lexer(std::string source) : source_(std::move(source)) {}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (!isAtEnd()) {
        skipWhitespace();
        if (isAtEnd()) break;
        
        tokenStartLine_ = line_;
        tokenStartColumn_ = column_;
        
        tokens.push_back(scanToken());
    }

    tokens.push_back(makeToken(TokenType::Eof, ""));
    return tokens;
}

bool Lexer::isAtEnd() const {
    return position_ >= source_.length();
}

char Lexer::advance() {
    char c = source_[position_++];
    if (c == '\n') {
        line_++;
        column_ = 1;
    } else {
        column_++;
    }
    return c;
}

char Lexer::peek() const {
    if (isAtEnd()) return '\0';
    return source_[position_];
}

void Lexer::skipWhitespace() {
    while (!isAtEnd()) {
        char c = peek();
        if (std::isspace(static_cast<unsigned char>(c))) {
            advance();
        } else {
            break;
        }
    }
}

Token Lexer::scanToken() {
    char c = peek();

    if (c == '"') return scanString();
    
    if (isIdentifierStart(c)) {
        return scanIdentifierOrKeyword();
    }

    // Single character tokens
    switch (c) {
        case '{': advance(); return makeToken(TokenType::LeftBrace, "{");
        case '}': advance(); return makeToken(TokenType::RightBrace, "}");
        case '(': advance(); return makeToken(TokenType::LeftParen, "(");
        case ')': advance(); return makeToken(TokenType::RightParen, ")");
        case ',': advance(); return makeToken(TokenType::Comma, ",");
    }

    throw std::runtime_error(
        "Unexpected character '" + std::string(1, c) + 
        "' at line " + std::to_string(line_) + 
        ", column " + std::to_string(column_)
    );
}

Token Lexer::scanString() {
    advance(); // Consume the opening '"'
    std::string value;

    while (!isAtEnd() && peek() != '"') {
        value += advance();
    }

    if (isAtEnd()) {
        throw std::runtime_error("Unterminated string at line " + std::to_string(tokenStartLine_));
    }

    advance(); // Consume the closing '"'
    return makeToken(TokenType::String, value);
}

Token Lexer::scanIdentifierOrKeyword() {
    std::string value;
    while (!isAtEnd() && isIdentifierPart(peek())) {
        value += advance();
    }

    auto it = KEYWORDS.find(value);
    if (it != KEYWORDS.end()) {
        return makeToken(it->second, value);
    }

    return makeToken(TokenType::Identifier, value);
}

Token Lexer::makeToken(TokenType type, std::string value) const {
    Token token;
    token.type = type;
    token.value = std::move(value);
    token.line = tokenStartLine_;
    token.column = tokenStartColumn_;
    return token;
}

bool Lexer::isIdentifierStart(char c) {
    return std::isalpha(static_cast<unsigned char>(c)) || c == '_';
}

bool Lexer::isIdentifierPart(char c) {
    return std::isalnum(static_cast<unsigned char>(c)) || c == '_';
}

} // namespace webforge
