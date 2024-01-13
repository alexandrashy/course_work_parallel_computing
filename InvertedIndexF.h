#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <iterator>
#include "vectorTest.h"

using std::chrono::nanoseconds;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;

// Function to tokenize a string into words
std::vector<std::string> tokenize(const std::string& str) {
    //std::istringstream iss(str);    
    std::regex wordRegex(("\\b(?!<|>|\\.|/|-)([A-Za-z]+)\\b"));
    std::smatch match;
    std::vector<std::string> tokens;

    std::string::const_iterator searchStart(str.cbegin());
    while (std::regex_search(searchStart, str.cend(), match, wordRegex)) {
        tokens.push_back(match[1].str());
        searchStart = match.suffix().first;
    }
    return tokens;
}

// Function to build the inverted index
std::unordered_map<std::string, std::vector<std::string>> buildInvertedIndex(std::unordered_map<std::string, std::vector<std::string>>& documents) {
    std::unordered_map<std::string, std::vector<std::string>> invertedIndex;
    for (auto a = documents.begin(); a != documents.end(); a++){
        for (int i = 0; i < a->second.size(); ++i) {
            const std::string& document = a->second[i];
            std::vector<std::string> words = tokenize(document);
            for (const std::string& word : words) {
                std::string lowercaseWord = word;
                lowercaseWord.erase(std::remove_if(lowercaseWord.begin(), lowercaseWord.end(), [](unsigned char c) {
                    return !std::isalpha(c);
                    }), lowercaseWord.end());
                if (lowercaseWord.size() <= 1) { continue; }
                
                std::transform(lowercaseWord.begin(), lowercaseWord.end(), lowercaseWord.begin(), ::tolower);

                invertedIndex[lowercaseWord].push_back(a->first);
            }
        }
    }
    return invertedIndex;
}

// Function to print the inverted index
void printInvertedIndex(const std::unordered_map<std::string, std::vector<int>>& invertedIndex) {
    int temp = 0;
    for (const auto& entry : invertedIndex) {
        if (entry.second.size() > 30) { continue; }
        std::cout << entry.first << ": ";
        std::cout << entry.second.size() << " - num of entry";
        for (int docId : entry.second) {
            std::cout << docId << " ";
        }
        temp++;
        if (temp > 10) { return; }
        std::cout << std::endl;
    }
}

std::unordered_map<std::string, std::vector<std::string>> CreateInvertedIndex(std::string folderPath) {
    // token();
    std::unordered_map<std::string, std::vector<std::string>> documents = CreateVector(folderPath);

    // Build the inverted index
    std::unordered_map<std::string, std::vector<std::string>> invertedIndex = buildInvertedIndex(documents);

    return invertedIndex;
}
