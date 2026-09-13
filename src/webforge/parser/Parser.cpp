#include "webforge/parser/Parser.h"

#include <utility>

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

    Token token = peek();
    throw ParseError(
        "Expected statement such as 'text', 'heading', 'image', 'link', or 'button' at line " +
        std::to_string(token.line) +
        ", column " +
        std::to_string(token.column) +
        "."
    );
}

ast::TextStatement Parser::parseTextStatement() {
    consume(TokenType::KeywordText, "Expected 'text' statement.");

    Token textToken = consume(
        TokenType::String,
        "Expected string after 'text'."
    );

    return ast::TextStatement{
        .text = textToken.value
    };
}

ast::HeadingStatement Parser::parseHeadingStatement() {
    consume(TokenType::KeywordHeading, "Expected 'heading' statement.");

    Token textToken = consume(
        TokenType::String,
        "Expected string after 'heading'."
    );

    return ast::HeadingStatement{
        .text = textToken.value
    };
}

ast::ImageStatement Parser::parseImageStatement() {
    consume(TokenType::KeywordImage, "Expected 'image' statement.");

    Token srcToken = consume(
        TokenType::String,
        "Expected image source string after 'image'."
    );

    Token altToken = consume(
        TokenType::String,
        "Expected alt text string after image source."
    );

    return ast::ImageStatement{
        .src = srcToken.value,
        .altText = altToken.value
    };
}

ast::LinkStatement Parser::parseLinkStatement() {
    consume(TokenType::KeywordLink, "Expected 'link' statement.");

    Token labelToken = consume(
        TokenType::String,
        "Expected link label string after 'link'."
    );

    Token hrefToken = consume(
        TokenType::String,
        "Expected link href string after link label."
    );

    return ast::LinkStatement{
        .label = labelToken.value,
        .href = hrefToken.value
    };
}

ast::ButtonStatement Parser::parseButtonStatement() {
    consume(TokenType::KeywordButton, "Expected 'button' statement.");

    Token labelToken = consume(
        TokenType::String,
        "Expected button label string after 'button'."
    );

    consume(
        TokenType::LeftBrace,
        "Expected '{' after button label."
    );

    ast::ButtonStatement button;
    button.label = labelToken.value;

    while (!check(TokenType::RightBrace) && !isAtEnd()) {
        button.handlers.push_back(parseEventHandler());
    }

    consume(
        TokenType::RightBrace,
        "Expected '}' to close button block."
    );

    return button;
}

ast::EventHandler Parser::parseEventHandler() {
    consume(TokenType::KeywordOn, "Expected 'on' inside button block.");

    Token eventToken = consume(
        TokenType::KeywordClick,
        "Currently only 'on click' events are supported."
    );

    consume(
        TokenType::LeftBrace,
        "Expected '{' after event name."
    );

    ast::EventHandler handler;
    handler.eventName = eventToken.value;

    while (!check(TokenType::RightBrace) && !isAtEnd()) {
        handler.actions.push_back(parseAction());
    }

    consume(
        TokenType::RightBrace,
        "Expected '}' to close event handler block."
    );

    return handler;
}

ast::Action Parser::parseAction() {
    consume(TokenType::KeywordAlert, "Expected 'alert' action inside event handler.");

    consume(
        TokenType::LeftParen,
        "Expected '(' after 'alert'."
    );

    Token messageToken = consume(
        TokenType::String,
        "Expected alert message string."
    );

    consume(
        TokenType::RightParen,
        "Expected ')' after alert message."
    );

    return ast::AlertAction{
        .message = messageToken.value
    };
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
