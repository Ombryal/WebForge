#pragma once

#include <string>
#include <vector>
#include <variant>

namespace webforge::ast {

struct TextStatement {
    std::string text;
};

struct HeadingStatement {
    std::string text;
};

struct ImageStatement {
    std::string src;
    std::string altText;
};

struct LinkStatement {
    std::string label;
    std::string href;
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
};

struct ListStatement {
    std::vector<std::string> items;
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
