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
    out << "</head>\n";
    out << "<body>\n";

    for (const auto& statement : page_.statements) {
        out << generateStatement(statement);
    }

    out << "</body>\n";
    out << "</html>\n";

    return out.str();
}

std::string HtmlGenerator::generateStatement(const ast::Statement& statement) const {
    return std::visit([&](const auto& value) -> std::string {
        using T = std::decay_t<decltype(value)>;

        if constexpr (std::is_same_v<T, ast::TextStatement>) {
            return generateText(value);
        } else if constexpr (std::is_same_v<T, ast::ButtonStatement>) {
            return generateButton(value);
        } else {
            static_assert(!sizeof(T*), "Unhandled ast::Statement alternative in codegen");
        }
    }, statement);
}

std::string HtmlGenerator::generateText(const ast::TextStatement& text) const {
    std::ostringstream out;
    out << "  <p>" << escapeHtml(text.text) << "</p>\n";
    return out.str();
}

std::string HtmlGenerator::generateButton(const ast::ButtonStatement& button) const {
    std::string js = generateClickJs(button);

    std::ostringstream out;
    out << "  <button";
    if (!js.empty()) {
        out << " onclick=\"" << escapeHtml(js) << "\"";
    }
    out << ">" << escapeHtml(button.label) << "</button>\n";
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
