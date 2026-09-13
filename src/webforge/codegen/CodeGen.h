#pragma once

#include <string>

#include "webforge/ast/Ast.h"

namespace webforge::codegen {

// Walks a parsed WebForge ast::Page and emits a standalone HTML document
// string. This is the first compiler backend milestone: static HTML output
// for `page` / `text`, plus inline onclick JS for `button { on click { ... } }`.
class HtmlGenerator {
public:
    explicit HtmlGenerator(const ast::Page& page);

    std::string generate() const;

private:
    std::string generateStatement(const ast::Statement& statement) const;
    std::string generateText(const ast::TextStatement& text) const;
    std::string generateHeading(const ast::HeadingStatement& heading) const;
    std::string generateImage(const ast::ImageStatement& image) const;
    std::string generateLink(const ast::LinkStatement& link) const;
    std::string generateButton(const ast::ButtonStatement& button) const;
    std::string generateList(const ast::ListStatement& list) const;
    std::string generateContainer(const ast::ContainerStatement& container) const;
    std::string generateContainerChild(const ast::ContainerChild& child) const;

    std::string generateClickJs(const ast::ButtonStatement& button) const;
    std::string generateActionJs(const ast::Action& action) const;

    static std::string escapeHtml(const std::string& input);
    static std::string escapeJsString(const std::string& input);

    const ast::Page& page_;
};

} // namespace webforge::codegen
