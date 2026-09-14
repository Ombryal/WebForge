#include "webforge/codegen/CodeGen.h"

#include <sstream>
#include <type_traits>
#include <variant>

namespace webforge::codegen {

HtmlGenerator::HtmlGenerator(const ast::Page& page) : page_(page) {}

std::string HtmlGenerator::generate() const {
    std::ostringstream out;

    out << "<!DOCTYPE html>\n";
    out << "<html lang=\"en\">\n";
    out << "<head>\n";
    out << "  <meta charset=\"UTF-8\">\n";
    out << "  <title>" << escapeHtml(page_.title) << "</title>\n";

    for (const auto& statement : page_.statements) {
        out << generateHeadExtra(statement);
    }

    out << "</head>\n";
    out << "<body>\n";

    for (const auto& statement : page_.statements) {
        out << generateStatement(statement);
    }

    out << "</body>\n";
    out << "</html>\n";

    return out.str();
}

std::string HtmlGenerator::generateHeadExtra(const ast::Statement& statement) const {
    return std::visit([&](const auto& value) -> std::string {
        using T = std::decay_t<decltype(value)>;

        if constexpr (std::is_same_v<T, ast::StylesheetStatement>) {
            return generateStylesheet(value);
        } else if constexpr (std::is_same_v<T, ast::MetaStatement>) {
            return generateMeta(value);
        } else {
            return ""; // Body statements have no <head> content.
        }
    }, statement);
}

std::string HtmlGenerator::generateStatement(const ast::Statement& statement) const {
    return std::visit([&](const auto& value) -> std::string {
        using T = std::decay_t<decltype(value)>;

        if constexpr (std::is_same_v<T, ast::TextStatement>) {
            return generateText(value);
        } else if constexpr (std::is_same_v<T, ast::HeadingStatement>) {
            return generateHeading(value);
        } else if constexpr (std::is_same_v<T, ast::ImageStatement>) {
            return generateImage(value);
        } else if constexpr (std::is_same_v<T, ast::LinkStatement>) {
            return generateLink(value);
        } else if constexpr (std::is_same_v<T, ast::ButtonStatement>) {
            return generateButton(value);
        } else if constexpr (std::is_same_v<T, ast::ListStatement>) {
            return generateList(value);
        } else if constexpr (std::is_same_v<T, ast::ContainerStatement>) {
            return generateContainer(value);
        } else if constexpr (std::is_same_v<T, ast::StylesheetStatement> ||
                              std::is_same_v<T, ast::MetaStatement>) {
            return ""; // Already emitted into <head> by generateHeadExtra.
        } else {
            static_assert(!sizeof(T*), "Unhandled ast::Statement alternative in codegen");
        }
    }, statement);
}

std::string HtmlGenerator::generateText(const ast::TextStatement& text) const {
    std::ostringstream out;
    out << "  <p" << buildElementAttributes(text.style) << ">"
        << escapeHtml(text.text) << "</p>\n";
    return out.str();
}

std::string HtmlGenerator::generateHeading(const ast::HeadingStatement& heading) const {
    std::ostringstream out;
    out << "  <h1" << buildElementAttributes(heading.style) << ">"
        << escapeHtml(heading.text) << "</h1>\n";
    return out.str();
}

std::string HtmlGenerator::generateImage(const ast::ImageStatement& image) const {
    std::ostringstream out;
    out << "  <img src=\"" << escapeHtml(image.src) << "\""
        << " alt=\"" << escapeHtml(image.altText) << "\""
        << buildElementAttributes(image.style) << ">\n";
    return out.str();
}

std::string HtmlGenerator::generateLink(const ast::LinkStatement& link) const {
    std::ostringstream out;
    out << "  <a href=\"" << escapeHtml(link.href) << "\"";

    if (!link.target.empty()) {
        out << " target=\"" << escapeHtml(link.target) << "\""
            << " rel=\"noopener noreferrer\"";
    }

    out << buildElementAttributes(link.style) << ">"
        << escapeHtml(link.label) << "</a>\n";
    return out.str();
}

std::string HtmlGenerator::generateButton(const ast::ButtonStatement& button) const {
    std::string js = generateClickJs(button);

    std::ostringstream out;
    out << "  <button";
    if (!js.empty()) {
        out << " onclick=\"" << escapeHtml(js) << "\"";
    }
    out << buildElementAttributes(button.style) << ">"
        << escapeHtml(button.label) << "</button>\n";
    return out.str();
}

std::string HtmlGenerator::generateList(const ast::ListStatement& list) const {
    std::ostringstream out;
    out << "  <ul" << buildElementAttributes(list.style) << ">\n";

    for (const auto& item : list.items) {
        out << "    <li>" << escapeHtml(item) << "</li>\n";
    }

    out << "  </ul>\n";
    return out.str();
}

std::string HtmlGenerator::generateContainer(const ast::ContainerStatement& container) const {
    std::ostringstream out;
    out << "  <div" << buildElementAttributes(container.style) << ">\n";

    for (const auto& child : container.children) {
        out << generateContainerChild(child);
    }

    out << "  </div>\n";
    return out.str();
}

std::string HtmlGenerator::generateContainerChild(const ast::ContainerChild& child) const {
    return std::visit([&](const auto& value) -> std::string {
        using T = std::decay_t<decltype(value)>;

        if constexpr (std::is_same_v<T, ast::TextStatement>) {
            return generateText(value);
        } else if constexpr (std::is_same_v<T, ast::HeadingStatement>) {
            return generateHeading(value);
        } else if constexpr (std::is_same_v<T, ast::ImageStatement>) {
            return generateImage(value);
        } else if constexpr (std::is_same_v<T, ast::LinkStatement>) {
            return generateLink(value);
        } else if constexpr (std::is_same_v<T, ast::ButtonStatement>) {
            return generateButton(value);
        } else if constexpr (std::is_same_v<T, ast::ListStatement>) {
            return generateList(value);
        } else if constexpr (std::is_same_v<T, ast::Box<ast::ContainerStatement>>) {
            return generateContainer(*value);
        } else {
            static_assert(!sizeof(T*), "Unhandled ast::ContainerChild alternative in codegen");
        }
    }, child);
}

std::string HtmlGenerator::generateStylesheet(const ast::StylesheetStatement& stylesheet) const {
    std::ostringstream out;
    out << "  <link rel=\"stylesheet\" href=\"" << escapeHtml(stylesheet.href) << "\">\n";
    return out.str();
}

std::string HtmlGenerator::generateMeta(const ast::MetaStatement& meta) const {
    std::ostringstream out;
    out << "  <meta name=\"" << escapeHtml(meta.name) << "\""
        << " content=\"" << escapeHtml(meta.content) << "\">\n";
    return out.str();
}

std::string HtmlGenerator::generateClickJs(const ast::ButtonStatement& button) const {
    std::ostringstream js;

    for (const auto& handler : button.handlers) {
        if (handler.eventName != "click") {
            continue; // Only 'on click' is supported at this milestone.
        }

        for (const auto& action : handler.actions) {
            js << generateActionJs(action);
        }
    }

    return js.str();
}

std::string HtmlGenerator::generateActionJs(const ast::Action& action) const {
    return std::visit([&](const auto& value) -> std::string {
        using T = std::decay_t<decltype(value)>;

        if constexpr (std::is_same_v<T, ast::AlertAction>) {
            std::ostringstream out;
            out << "alert('" << escapeJsString(value.message) << "');";
            return out.str();
        } else {
            static_assert(!sizeof(T*), "Unhandled ast::Action alternative in codegen");
        }
    }, action);
}

std::string HtmlGenerator::buildElementAttributes(const ast::StyleProperties& style) {
    std::string idValue;
    std::string classValue;
    ast::StyleProperties remainingStyle;

    for (const auto& property : style) {
        if (property.name == "id" && idValue.empty()) {
            idValue = property.value;
        } else if (property.name == "class") {
            if (!classValue.empty()) {
                classValue += " ";
            }
            classValue += property.value;
        } else {
            remainingStyle.push_back(property);
        }
    }

    std::ostringstream out;

    if (!idValue.empty()) {
        out << " id=\"" << escapeHtml(idValue) << "\"";
    }

    if (!classValue.empty()) {
        out << " class=\"" << escapeHtml(classValue) << "\"";
    }

    out << buildStyleAttribute(remainingStyle);

    return out.str();
}

std::string HtmlGenerator::buildStyleAttribute(const ast::StyleProperties& remainingStyle) {
    if (remainingStyle.empty()) {
        return "";
    }

    std::ostringstream css;
    for (const auto& property : remainingStyle) {
        css << property.name << ":" << property.value << ";";
    }

    std::ostringstream out;
    out << " style=\"" << escapeHtml(css.str()) << "\"";
    return out.str();
}

std::string HtmlGenerator::escapeHtml(const std::string& input) {
    std::string out;
    out.reserve(input.size());

    for (char c : input) {
        switch (c) {
            case '&':  out += "&amp;";  break;
            case '<':  out += "&lt;";   break;
            case '>':  out += "&gt;";   break;
            case '"':  out += "&quot;"; break;
            case '\'': out += "&#39;";  break;
            default:   out += c;        break;
        }
    }

    return out;
}

std::string HtmlGenerator::escapeJsString(const std::string& input) {
    std::string out;
    out.reserve(input.size());

    for (char c : input) {
        switch (c) {
            case '\\': out += "\\\\"; break;
            case '\'': out += "\\'";  break;
            case '\n': out += "\\n";  break;
            default:   out += c;      break;
        }
    }

    return out;
}

} // namespace webforge::codegen
