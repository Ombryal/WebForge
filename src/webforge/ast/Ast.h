#pragma once

#include <string>
#include <vector>
#include <variant>

namespace webforge::ast {

struct TextStatement {
    std::string text;
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

using Statement = std::variant<TextStatement, ButtonStatement>;

struct Page {
    std::string title;
    std::vector<Statement> statements;
};

} // namespace webforge::ast
