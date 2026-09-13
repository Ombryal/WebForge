#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>

#include "webforge/lexer/Lexer.h"

std::string readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + path);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::string tokenTypeToString(webforge::TokenType type) {
    switch (type) {
        case webforge::TokenType::Eof: return "Eof";
        case webforge::TokenType::Unknown: return "Unknown";
        case webforge::TokenType::KeywordPage: return "KeywordPage";
        case webforge::TokenType::KeywordText: return "KeywordText";
        case webforge::TokenType::KeywordButton: return "KeywordButton";
        case webforge::TokenType::KeywordOn: return "KeywordOn";
        case webforge::TokenType::KeywordClick: return "KeywordClick";
        case webforge::TokenType::KeywordAlert: return "KeywordAlert";
        case webforge::TokenType::Identifier: return "Identifier";
        case webforge::TokenType::String: return "String";
        case webforge::TokenType::LeftBrace: return "LeftBrace";
        case webforge::TokenType::RightBrace: return "RightBrace";
        case webforge::TokenType::LeftParen: return "LeftParen";
        case webforge::TokenType::RightParen: return "RightParen";
        case webforge::TokenType::Comma: return "Comma";
    }
    return "Unknown";
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

        std::cout << "Successfully lexed " << inputPath << ":\n\n";
        for (const auto& token : tokens) {
            std::cout << "[" << token.line << ":" << token.column << "] "
                      << tokenTypeToString(token.type);
            if (!token.value.empty()) {
                std::cout << " -> \"" << token.value << "\"";
            }
            std::cout << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
