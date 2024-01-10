#include <iostream>
#include <vector>
#include <regex>

std::vector<std::string> tokeniz(const std::string& str) {
    std::regex wordRegex("\\b(?!<|>|\\.|/|-)([A-Za-z]+)\\b");
    std::smatch match;
    std::vector<std::string> tokens;

    std::string::const_iterator searchStart(str.cbegin());
    while (std::regex_search(searchStart, str.cend(), match, wordRegex)) {
        tokens.push_back(match[1].str());
        searchStart = match.suffix().first;
    }

    return tokens;
}

int token() {
    std::string inputString = "too <br /> much <br /> phrase---voice....and";

    std::vector<std::string> tokens = tokeniz(inputString);

    // Print the resulting tokens
    for (const auto& token : tokens) {
        std::cout << token << std::endl;
    }

    return 0;
}
