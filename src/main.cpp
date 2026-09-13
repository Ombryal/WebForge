#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "webforge/codegen/CodeGen.h"
#include "webforge/lexer/Lexer.h"
#include "webforge/parser/Parser.h"

namespace {

std::string readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + path);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void writeFile(const std::string& path, const std::string& contents) {
    std::ofstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Could not write file: " + path);
    }

    file << contents;
}

// Swaps a .wf extension (or appends one) for .html, e.g. "app.wf" -> "app.html".
std::string defaultOutputPath(const std::string& inputPath) {
    const std::string suffix = ".wf";
    if (inputPath.size() >= suffix.size() &&
        inputPath.compare(inputPath.size() - suffix.size(), suffix.size(), suffix) == 0) {
        return inputPath.substr(0, inputPath.size() - suffix.size()) + ".html";
    }
    return inputPath + ".html";
}

} // namespace

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "usage: webforge <input.wf> [output.html]\n";
        return 1;
    }

    const std::string inputPath = argv[1];
    const std::string outputPath = argc >= 3 ? argv[2] : defaultOutputPath(inputPath);

    try {
        std::string source = readFile(inputPath);

        webforge::Lexer lexer(source);
        std::vector<webforge::Token> tokens = lexer.tokenize();

        webforge::Parser parser(std::move(tokens));
        webforge::ast::Page page = parser.parse();

        webforge::codegen::HtmlGenerator generator(page);
        std::string html = generator.generate();

        writeFile(outputPath, html);

        std::cout << "Compiled " << inputPath << " -> " << outputPath << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
