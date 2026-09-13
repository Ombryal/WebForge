#pragma once

#include <string>
#include <vector>
#include <variant>

namespace webforge::ast {

// A single "property" "value" pair, rendered as one entry of an inline
// style attribute (e.g. {"color", "blue"} -> style="color:blue;").
struct StyleProperty {
    std::string name;
    std::string value;
};

using StyleProperties = std::vector<StyleProperty>;

struct TextStatement {
    std::string text;
    StyleProperties style;
};

struct HeadingStatement {
    std::string text;
    StyleProperties style;
};

struct ImageStatement {
    std::string src;
    std::string altText;
    StyleProperties style;
};

struct LinkStatement {
    std::string label;
    std::string href;
    std::string target; // empty means no target attribute
    StyleProperties style;
};

struct AlertAction {
    std::string message;
};

using Action = std::variant<AlertAction>;

struct EventHandler {
    std::string eventName;
    std::vector<Action> actions;
};

struct ButtonStatement {
    std::string label;
    std::vector<EventHandler> handlers;
    StyleProperties style;
};

struct ListStatement {
    std::vector<std::string> items;
    StyleProperties style;
};

// What a container is allowed to hold. Deliberately excludes ContainerStatement
// itself for now — nested containers aren't supported yet, since a container
// holding itself needs a recursive-variant design we haven't introduced.
using ContainerChild = std::variant<
    TextStatement,
    HeadingStatement,
    ImageStatement,
    LinkStatement,
    ButtonStatement,
    ListStatement>;

struct ContainerStatement {
    std::vector<ContainerChild> children;
    StyleProperties style;
};

using Statement = std::variant<
    TextStatement,
    HeadingStatement,
    ImageStatement,
    LinkStatement,
    ButtonStatement,
    ListStatement,
    ContainerStatement>;

struct Page {
    std::string title;
    std::vector<Statement> statements;
};

} // namespace webforge::ast
