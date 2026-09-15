#pragma once

#include <memory>
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

// An escape hatch: `html` is emitted completely unescaped, unlike every
// other statement. Valid both at page level and inside a container.
struct RawHtmlStatement {
    std::string html;
};

struct ContainerStatement;

// A heap-boxed, copyable holder for a value of type T. Exists to break the
// recursive-size cycle between ContainerStatement and ContainerChild: a
// container can now hold another container, but std::variant needs every
// alternative to be a complete, fixed-size type, and a container holding
// itself by value would be infinitely large. Boxing the recursive case in a
// unique_ptr gives it a fixed size (that of a pointer) while still behaving
// like a normal value — copyable, with *box / box-> access. Constructors
// take T by reference rather than by value specifically so Box<T> stays
// well-formed while T (ContainerStatement) is still an incomplete type,
// which it is at the point ContainerChild is declared below.
template <typename T>
class Box {
public:
    explicit Box(const T& value) : value_(std::make_unique<T>(value)) {}
    explicit Box(T&& value) : value_(std::make_unique<T>(std::move(value))) {}
    Box(const Box& other) : value_(std::make_unique<T>(*other.value_)) {}
    Box(Box&& other) noexcept = default;

    Box& operator=(const Box& other) {
        value_ = std::make_unique<T>(*other.value_);
        return *this;
    }
    Box& operator=(Box&& other) noexcept = default;

    ~Box() = default;

    T& operator*() { return *value_; }
    const T& operator*() const { return *value_; }
    T* operator->() { return value_.get(); }
    const T* operator->() const { return value_.get(); }

private:
    std::unique_ptr<T> value_;
};

// What a container is allowed to hold. A nested container is boxed (see
// Box<T> above), so a container can now contain another container, to
// arbitrary depth.
using ContainerChild = std::variant<
    TextStatement,
    HeadingStatement,
    ImageStatement,
    LinkStatement,
    ButtonStatement,
    ListStatement,
    RawHtmlStatement,
    Box<ContainerStatement>>;

struct ContainerStatement {
    std::vector<ContainerChild> children;
    StyleProperties style;
};

// Page-level only: emitted into <head>, not valid inside a container.
struct StylesheetStatement {
    std::string href;
};

// Page-level only: emitted into <head>, not valid inside a container.
struct MetaStatement {
    std::string name;
    std::string content;
};

// Page-level only: emitted into <head>, not valid inside a container.
struct FaviconStatement {
    std::string href;
};

using Statement = std::variant<
    TextStatement,
    HeadingStatement,
    ImageStatement,
    LinkStatement,
    ButtonStatement,
    ListStatement,
    RawHtmlStatement,
    ContainerStatement,
    StylesheetStatement,
    MetaStatement,
    FaviconStatement>;

struct Page {
    std::string title;
    std::vector<Statement> statements;
};

} // namespace webforge::ast
