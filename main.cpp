#include <libgen.h>

#include <cstring>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>

static void convertOllamaToJinja(const std::string & inputFilePath, const std::string & outputFilePath) {
    std::ifstream inputFile(inputFilePath);
    std::ofstream outputFile(outputFilePath);
    if (!inputFile.is_open() || !outputFile.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        // Replace Ollama specific syntax with Jinja syntax
        line = std::regex_replace(line, std::regex(R"(\{\{-)"), "{{");
        line = std::regex_replace(line, std::regex(R"(-\}\})"), "}}");
        line = std::regex_replace(line, std::regex(R"(\{\{)"), "{{");
        line = std::regex_replace(line, std::regex(R"(\}\})"), "}}");
        outputFile << line << std::endl;
    }
}

static void convertJinjaToOllama(const std::string & inputFilePath, const std::string & outputFilePath) {
    std::ifstream inputFile(inputFilePath);
    std::ofstream outputFile(outputFilePath);
    if (!inputFile.is_open() || !outputFile.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        // Replace Jinja specific syntax with Ollama syntax
        line = std::regex_replace(line, std::regex(R"(\{\{)"), "{{-");
        line = std::regex_replace(line, std::regex(R"(\}\})"), "-}}");
        outputFile << line << std::endl;
    }
}

int main(int argc, char * argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        return 1;
    }

    std::string  inputFilePath  = argv[1];
    std::string  outputFilePath = argv[2];
    const char * base           = basename(argv[0]);
    if (strcmp(base, "ollama2jinja") == 0) {
        convertOllamaToJinja(inputFilePath, outputFilePath);
    } else if (strcmp(base, "jinja2ollama") == 0) {
        convertJinjaToOllama(inputFilePath, outputFilePath);
    } else {
        std::cerr << "Invalid program name. Use 'ollama2jinja' or 'jinja2ollama'." << std::endl;
        return 1;
    }

    std::cout << "Conversion completed successfully!" << std::endl;
    return 0;
}
