#include "webforge/parser/Parser.h"

// Every individual statement parser, moved out of Parser.cpp verbatim to
// keep that file focused on token-stream plumbing and top-level dispatch.

namespace webforge {

ast::TextStatement Parser::parseTextStatement() {
    consume(TokenType::KeywordText, "Expected 'text' statement.");

    Token textToken = consume(
        TokenType::String,
        "Expected string after 'text'."
    );

    return ast::TextStatement{
        .text = textToken.value,
        .style = parseStyleBlockIfPresent()
    };
}

ast::HeadingStatement Parser::parseHeadingStatement() {
    consume(TokenType::KeywordHeading, "Expected 'heading' statement.");

    Token textToken = consume(
        TokenType::String,
        "Expected string after 'heading'."
    );

    return ast::HeadingStatement{
        .text = textToken.value,
        .style = parseStyleBlockIfPresent()
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
        .altText = altToken.value,
        .style = parseStyleBlockIfPresent()
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

    std::string target;
    if (check(TokenType::String)) {
        target = advance().value;
    }

    return ast::LinkStatement{
        .label = labelToken.value,
        .href = hrefToken.value,
        .target = target,
        .style = parseStyleBlockIfPresent()
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
        if (check(TokenType::String)) {
            button.style.push_back(parseStyleProperty());
        } else {
            button.handlers.push_back(parseEventHandler());
        }
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

ast::ListStatement Parser::parseListStatement() {
    consume(TokenType::KeywordList, "Expected 'list' statement.");
    consume(TokenType::LeftBrace, "Expected '{' after 'list'.");

    ast::ListStatement list;

    while (!check(TokenType::RightBrace) && !isAtEnd()) {
        if (check(TokenType::String)) {
            list.style.push_back(parseStyleProperty());
            continue;
        }

        consume(TokenType::KeywordItem, "Expected 'item' inside list block.");

        Token itemToken = consume(
            TokenType::String,
            "Expected string after 'item'."
        );

        list.items.push_back(itemToken.value);
    }

    consume(TokenType::RightBrace, "Expected '}' to close list block.");

    return list;
}

ast::ContainerStatement Parser::parseContainerStatement() {
    consume(TokenType::KeywordContainer, "Expected 'container' statement.");
    consume(TokenType::LeftBrace, "Expected '{' after 'container'.");

    ast::ContainerStatement container;

    while (!check(TokenType::RightBrace) && !isAtEnd()) {
        if (check(TokenType::String)) {
            container.style.push_back(parseStyleProperty());
            continue;
        }

        container.children.push_back(parseContainerChild());
    }

    consume(TokenType::RightBrace, "Expected '}' to close container block.");

    return container;
}

ast::ContainerChild Parser::parseContainerChild() {
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

    Token token = peek();

    if (check(TokenType::KeywordContainer)) {
        throw ParseError(
            "Nested containers are not supported yet, at line " +
            std::to_string(token.line) +
            ", column " +
            std::to_string(token.column) +
            "."
        );
    }

    throw ParseError(
        "Expected statement such as 'text', 'heading', 'image', 'link', 'button', or 'list' inside container at line " +
        std::to_string(token.line) +
        ", column " +
        std::to_string(token.column) +
        "."
    );
}

ast::StyleProperties Parser::parseStyleBlockIfPresent() {
    if (!check(TokenType::LeftBrace)) {
        return {};
    }

    consume(TokenType::LeftBrace, "Expected '{' to start style block.");

    ast::StyleProperties style;

    while (!check(TokenType::RightBrace) && !isAtEnd()) {
        style.push_back(parseStyleProperty());
    }

    consume(TokenType::RightBrace, "Expected '}' to close style block.");

    return style;
}

ast::StyleProperty Parser::parseStyleProperty() {
    Token nameToken = consume(
        TokenType::String,
        "Expected style property name string."
    );

    Token valueToken = consume(
        TokenType::String,
        "Expected style property value string after '" + nameToken.value + "'."
    );

    return ast::StyleProperty{
        .name = nameToken.value,
        .value = valueToken.value
    };
}

ast::StylesheetStatement Parser::parseStylesheetStatement() {
    consume(TokenType::KeywordStylesheet, "Expected 'stylesheet' statement.");

    Token hrefToken = consume(
        TokenType::String,
        "Expected stylesheet path string after 'stylesheet'."
    );

    return ast::StylesheetStatement{
        .href = hrefToken.value
    };
}

ast::MetaStatement Parser::parseMetaStatement() {
    consume(TokenType::KeywordMeta, "Expected 'meta' statement.");

    Token nameToken = consume(
        TokenType::String,
        "Expected meta name string after 'meta'."
    );

    Token contentToken = consume(
        TokenType::String,
        "Expected meta content string after meta name."
    );

    return ast::MetaStatement{
        .name = nameToken.value,
        .content = contentToken.value
    };
}

} // namespace webforge
