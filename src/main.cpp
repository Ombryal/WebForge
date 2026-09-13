#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#include "webforge/lexer/Lexer.h"
#include "webforge/parser/Parser.h"

std::string readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + path);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void printIndent(int indent) {
    std::cout << std::string(static_cast<std::size_t>(indent) * 2, ' ');
}

void printAction(const webforge::ast::Action& action, int indent) {
    std::visit([&](const auto& value) {
        using T = std::decay_t<decltype(value)>;

        if constexpr (std::is_same_v<T, webforge::ast::AlertAction>) {
            printIndent(indent);
            std::cout << "alert(\"" << value.message << "\")\n";
        }
    }, action);
}

void printEventHandler(const webforge::ast::EventHandler& handler, int indent) {
    printIndent(indent);
    std::cout << "on " << handler.eventName << " {\n";

    for (const auto& action : handler.actions) {
        printAction(action, indent + 1);
    }

    printIndent(indent);
    std::cout << "}\n";
}

void printStatement(const webforge::ast::Statement& statement, int indent) {
    std::visit([&](const auto& value) {
        using T = std::decay_t<decltype(value)>;

        if constexpr (std::is_same_v<T, webforge::ast::TextStatement>) {
            printIndent(indent);
            std::cout << "text \"" << value.text << "\"\n";
        } else if constexpr (std::is_same_v<T, webforge::ast::ButtonStatement>) {
            printIndent(indent);
            std::cout << "button \"" << value.label << "\" {\n";

            for (const auto& handler : value.handlers) {
                printEventHandler(handler, indent + 1);
            }

            printIndent(indent);
            std::cout << "}\n";
        }
    }, statement);
}

void printPage(const webforge::ast::Page& page) {
    std::cout << "page \"" << page.title << "\"\n";
    std::cout << "\n";

    for (const auto& statement : page.statements) {
        printStatement(statement, 0);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "usage: webforge <input.wf>\n";
        return 1;
    }

    const std::string inputPath = argv[1];

    try {
        std::string source = readFile(inputPath);

        webforge::Lexer lexer(source);
        std::vector<webforge::Token> tokens = lexer.tokenize();

        webforge::Parser parser(std::move(tokens));
        webforge::ast::Page page = parser.parse();

        printPage(page);

        std::cout << "\nParse OK\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
