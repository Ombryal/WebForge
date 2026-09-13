#include <cassert>
#include <string>
#include <variant>

#include "TestCases.h"
#include "webforge/lexer/Lexer.h"
#include "webforge/parser/Parser.h"

using namespace webforge;

void testParserBasicPage() {
    std::string source =
        "page \"Hello World\"\n"
        "\n"
        "text \"Hello, world!\"\n";

    Lexer lexer(source);
    Parser parser(lexer.tokenize());

    ast::Page page = parser.parse();

    assert(page.title == "Hello World");
    assert(page.statements.size() == 1);

    const ast::TextStatement* text =
        std::get_if<ast::TextStatement>(&page.statements[0]);

    assert(text != nullptr);
    assert(text->text == "Hello, world!");
}

void testParserButtonProgram() {
    std::string source =
        "page \"Hello\"\n"
        "\n"
        "button \"Click me\" {\n"
        "    on click {\n"
        "        alert(\"Hi\")\n"
        "    }\n"
        "}\n";

    Lexer lexer(source);
    Parser parser(lexer.tokenize());

    ast::Page page = parser.parse();

    assert(page.title == "Hello");
    assert(page.statements.size() == 1);

    const ast::ButtonStatement* button =
        std::get_if<ast::ButtonStatement>(&page.statements[0]);

    assert(button != nullptr);
    assert(button->label == "Click me");
    assert(button->handlers.size() == 1);

    const ast::EventHandler& handler = button->handlers[0];

    assert(handler.eventName == "click");
    assert(handler.actions.size() == 1);

    const ast::AlertAction* alert =
        std::get_if<ast::AlertAction>(&handler.actions[0]);

    assert(alert != nullptr);
    assert(alert->message == "Hi");
}

void testParserBasicElements() {
    std::string source =
        "page \"Hello\"\n"
        "\n"
        "heading \"Welcome\"\n"
        "image \"cat.png\" \"A cat\"\n"
        "link \"Docs\" \"https://example.com\"\n";

    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    ast::Page page = parser.parse();

    assert(page.statements.size() == 3);

    const ast::HeadingStatement* heading =
        std::get_if<ast::HeadingStatement>(&page.statements[0]);
    assert(heading != nullptr);
    assert(heading->text == "Welcome");

    const ast::ImageStatement* image =
        std::get_if<ast::ImageStatement>(&page.statements[1]);
    assert(image != nullptr);
    assert(image->src == "cat.png");
    assert(image->altText == "A cat");

    const ast::LinkStatement* link =
        std::get_if<ast::LinkStatement>(&page.statements[2]);
    assert(link != nullptr);
    assert(link->label == "Docs");
    assert(link->href == "https://example.com");
}

void testParserListStatement() {
    std::string source =
        "page \"Hello\"\n"
        "\n"
        "list {\n"
        "    item \"One\"\n"
        "    item \"Two\"\n"
        "}\n";

    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    ast::Page page = parser.parse();

    assert(page.statements.size() == 1);

    const ast::ListStatement* list =
        std::get_if<ast::ListStatement>(&page.statements[0]);

    assert(list != nullptr);
    assert(list->items.size() == 2);
    assert(list->items[0] == "One");
    assert(list->items[1] == "Two");
}

void testParserContainerStatement() {
    std::string source =
        "page \"Hello\"\n"
        "\n"
        "container {\n"
        "    heading \"Welcome\"\n"
        "    text \"Body copy\"\n"
        "}\n";

    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    ast::Page page = parser.parse();

    assert(page.statements.size() == 1);

    const ast::ContainerStatement* container =
        std::get_if<ast::ContainerStatement>(&page.statements[0]);

    assert(container != nullptr);
    assert(container->children.size() == 2);

    const ast::HeadingStatement* heading =
        std::get_if<ast::HeadingStatement>(&container->children[0]);
    assert(heading != nullptr);
    assert(heading->text == "Welcome");

    const ast::TextStatement* text =
        std::get_if<ast::TextStatement>(&container->children[1]);
    assert(text != nullptr);
    assert(text->text == "Body copy");
}

void testParserRejectsNestedContainer() {
    std::string source =
        "page \"Hello\"\n"
        "\n"
        "container {\n"
        "    container {\n"
        "        text \"Nested\"\n"
        "    }\n"
        "}\n";

    Lexer lexer(source);
    Parser parser(lexer.tokenize());

    bool threw = false;
    try {
        parser.parse();
    } catch (const ParseError&) {
        threw = true;
    }

    assert(threw);
}

void testParserStyleOnLeafStatement() {
    std::string source =
        "page \"Hello\"\n"
        "\n"
        "text \"Styled\" {\n"
        "    \"color\" \"blue\"\n"
        "    \"font-size\" \"20px\"\n"
        "}\n";

    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    ast::Page page = parser.parse();

    const ast::TextStatement* text =
        std::get_if<ast::TextStatement>(&page.statements[0]);

    assert(text != nullptr);
    assert(text->style.size() == 2);
    assert(text->style[0].name == "color");
    assert(text->style[0].value == "blue");
    assert(text->style[1].name == "font-size");
    assert(text->style[1].value == "20px");
}

void testParserStyleInsideButtonAndContainer() {
    std::string source =
        "page \"Hello\"\n"
        "\n"
        "container {\n"
        "    \"background\" \"gray\"\n"
        "    button \"Click me\" {\n"
        "        \"color\" \"white\"\n"
        "        on click {\n"
        "            alert(\"Hi\")\n"
        "        }\n"
        "    }\n"
        "}\n";

    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    ast::Page page = parser.parse();

    const ast::ContainerStatement* container =
        std::get_if<ast::ContainerStatement>(&page.statements[0]);

    assert(container != nullptr);
    assert(container->style.size() == 1);
    assert(container->style[0].name == "background");
    assert(container->children.size() == 1);

    const ast::ButtonStatement* button =
        std::get_if<ast::ButtonStatement>(&container->children[0]);

    assert(button != nullptr);
    assert(button->style.size() == 1);
    assert(button->style[0].name == "color");
    assert(button->handlers.size() == 1);
}

void testParserLinkTarget() {
    std::string source =
        "page \"Hello\"\n"
        "\n"
        "link \"Docs\" \"https://example.com\" \"_blank\"\n";

    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    ast::Page page = parser.parse();

    const ast::LinkStatement* link =
        std::get_if<ast::LinkStatement>(&page.statements[0]);

    assert(link != nullptr);
    assert(link->target == "_blank");
}

void testParserStylesheetAndMeta() {
    std::string source =
        "page \"Hello\"\n"
        "\n"
        "stylesheet \"styles.css\"\n"
        "meta \"description\" \"A test page.\"\n";

    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    ast::Page page = parser.parse();

    assert(page.statements.size() == 2);

    const ast::StylesheetStatement* stylesheet =
        std::get_if<ast::StylesheetStatement>(&page.statements[0]);
    assert(stylesheet != nullptr);
    assert(stylesheet->href == "styles.css");

    const ast::MetaStatement* meta =
        std::get_if<ast::MetaStatement>(&page.statements[1]);
    assert(meta != nullptr);
    assert(meta->name == "description");
    assert(meta->content == "A test page.");
}
