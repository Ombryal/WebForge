#pragma once

#include <string>

namespace webforge {

enum class TokenType {
    Eof,
    Unknown,

    // Keywords
    KeywordPage,
    KeywordText,
    KeywordButton,
    KeywordOn,
    KeywordClick,
    KeywordAlert,

    // Literals and identifiers
    Identifier,
    String,

    // Punctuation
    LeftBrace,
    RightBrace,
    LeftParen,
    RightParen,
    Comma
};

struct Token {
    TokenType type = TokenType::Unknown;
    std::string value;
    int line = 0;
    int column = 0;
};

} // namespace webforge
