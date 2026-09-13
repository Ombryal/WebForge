#pragma once

#include <vector>
#include <stdexcept>

#include "webforge/lexer/Token.h"
#include "webforge/ast/Ast.h"

namespace webforge {

class ParseError : public std::runtime_error {
public:
    explicit ParseError(const std::string& message) : std::runtime_error(message) {}
};

class Parser {
public:
    explicit Parser(std::vector<Token> tokens);

    ast::Page parse();

private:
    ast::Page parsePage();
    ast::Statement parseStatement();
    ast::TextStatement parseTextStatement();
    ast::HeadingStatement parseHeadingStatement();
    ast::ImageStatement parseImageStatement();
    ast::LinkStatement parseLinkStatement();
    ast::ButtonStatement parseButtonStatement();
    ast::EventHandler parseEventHandler();
    ast::Action parseAction();
    ast::ListStatement parseListStatement();
    ast::ContainerStatement parseContainerStatement();
    ast::ContainerChild parseContainerChild();

    bool match(TokenType type);
    bool check(TokenType type) const;
    Token advance();
    Token consume(TokenType type, const std::string& errorMessage);
    bool isAtEnd() const;
    const Token& peek() const;
    const Token& previous() const;

    std::vector<Token> tokens_;
    std::size_t current_ = 0;
};

} // namespace webforge
