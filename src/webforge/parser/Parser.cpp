#include "webforge/parser/Parser.h"

#include <utility>

// Individual statement parsers (parseTextStatement, parseButtonStatement,
// parseContainerChild, the style-block helpers, etc.) live in
// ParserStatements.cpp. This file keeps the token-stream plumbing and the
// top-level page/statement dispatch.

namespace webforge {

Parser::Parser(std::vector<Token> tokens)
    : tokens_(std::move(tokens)) {}

ast::Page Parser::parse() {
    return parsePage();
}

ast::Page Parser::parsePage() {
    consume(TokenType::KeywordPage, "Expected 'page' at the beginning of a WebForge program.");

    Token titleToken = consume(
        TokenType::String,
        "Expected page title string after 'page'."
    );

    ast::Page page;
    page.title = titleToken.value;

    while (!isAtEnd()) {
        page.statements.push_back(parseStatement());
    }

    return page;
}

ast::Statement Parser::parseStatement() {
    if (check(TokenType::KeywordText)) {
        return parseTextStatement();
    }

    if (check(TokenType::KeywordHeading)) {
        return parseHeadingStatement();
    }

    if (check(TokenType::KeywordImage)) {
        return parseImageStatement();
    }

    if (check(TokenType::KeywordLink)) {
        return parseLinkStatement();
    }

    if (check(TokenType::KeywordButton)) {
        return parseButtonStatement();
    }

    if (check(TokenType::KeywordList)) {
        return parseListStatement();
    }

    if (check(TokenType::KeywordContainer)) {
        return parseContainerStatement();
    }

    if (check(TokenType::KeywordStylesheet)) {
        return parseStylesheetStatement();
    }

    if (check(TokenType::KeywordMeta)) {
        return parseMetaStatement();
    }

    if (check(TokenType::KeywordFavicon)) {
        return parseFaviconStatement();
    }

    if (check(TokenType::KeywordRaw)) {
        return parseRawHtmlStatement();
    }

    Token token = peek();
    throw ParseError(
        "Expected statement such as 'text', 'heading', 'image', 'link', 'button', 'list', 'container', 'stylesheet', 'meta', 'favicon', or 'raw' at line " +
        std::to_string(token.line) +
        ", column " +
        std::to_string(token.column) +
        "."
    );
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }

    return false;
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) {
        return false;
    }

    return peek().type == type;
}

Token Parser::advance() {
    const Token& token = tokens_[current_];

    if (!isAtEnd()) {
        current_++;
    }

    return token;
}

Token Parser::consume(TokenType type, const std::string& errorMessage) {
    if (check(type)) {
        return advance();
    }

    Token token = peek();

    std::string found = token.value.empty() ? "<empty>" : token.value;

    throw ParseError(
        errorMessage +
        " Found '" + found + "'" +
        " at line " + std::to_string(token.line) +
        ", column " + std::to_string(token.column) +
        "."
    );
}

bool Parser::isAtEnd() const {
    return peek().type == TokenType::Eof;
}

const Token& Parser::peek() const {
    return tokens_[current_];
}

const Token& Parser::previous() const {
    if (current_ == 0) {
        return tokens_[0];
    }

    return tokens_[current_ - 1];
}

} // namespace webforge
