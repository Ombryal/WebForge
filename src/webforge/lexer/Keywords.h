#pragma once

#include <string>
#include <unordered_map>

#include "webforge/lexer/Token.h"

namespace webforge {

inline const std::unordered_map<std::string, TokenType> KEYWORDS = {
    {"page",       TokenType::KeywordPage},
    {"text",       TokenType::KeywordText},
    {"button",     TokenType::KeywordButton},
    {"on",         TokenType::KeywordOn},
    {"click",      TokenType::KeywordClick},
    {"alert",      TokenType::KeywordAlert},
    {"heading",    TokenType::KeywordHeading},
    {"image",      TokenType::KeywordImage},
    {"link",       TokenType::KeywordLink},
    {"container",  TokenType::KeywordContainer},
    {"list",       TokenType::KeywordList},
    {"item",       TokenType::KeywordItem},
    {"stylesheet", TokenType::KeywordStylesheet},
    {"meta",       TokenType::KeywordMeta}
};

} // namespace webforge
