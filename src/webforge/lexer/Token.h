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
    KeywordHeading,
    KeywordImage,
    KeywordLink,
    KeywordContainer,
    KeywordList,
    KeywordItem,
    KeywordStylesheet,
    KeywordMeta,
    KeywordHover,
    KeywordFavicon,
    KeywordRaw,
    KeywordOrdered,
    KeywordNav,
    KeywordHeader,
    KeywordFooter,
    KeywordMain,
    KeywordSection,
    KeywordArticle,

    // Literals and identifiers
    Identifier,
    String,
    Number,

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
