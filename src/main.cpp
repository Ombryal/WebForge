#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "usage: webforge <input.wf>\n";
        return 1;
    }

    const std::string inputPath = argv[1];

    std::cout << "WebForge bootstrap\n";
    std::cout << "input: " << inputPath << '\n';

    return 0;
}
