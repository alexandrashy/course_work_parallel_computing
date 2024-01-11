#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <iterator>
#include "vectorTest.h"

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
    //return { std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{} };
}

// Function to build the inverted index
std::unordered_map<std::string, std::vector<int>> buildInvertedIndex(const std::vector<std::string>& documents) {
    std::unordered_map<std::string, std::vector<int>> invertedIndex;

    for (int i = 0; i < documents.size(); ++i) {
        const std::string& document = documents[i];
        std::vector<std::string> words = tokenize(document);

        for (const std::string& word : words) {
            std::string lowercaseWord = word;
            lowercaseWord.erase(std::remove_if(lowercaseWord.begin(), lowercaseWord.end(), [](unsigned char c) {
                return !std::isalpha(c);
                }), lowercaseWord.end());
            if (lowercaseWord.size() <= 1) { continue; }
            // Convert the word to lowercase (for case-insensitive indexing)
            std::transform(lowercaseWord.begin(), lowercaseWord.end(), lowercaseWord.begin(), ::tolower);

            // Update the inverted index
            invertedIndex[lowercaseWord].push_back(i);
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

std::unordered_map<std::string, std::vector<int>> CretaeInvertedIndex(std::string folderPath) {
    // token();
    std::vector<std::string> documents = CreateVector(folderPath);

    // Build the inverted index
    std::unordered_map<std::string, std::vector<int>> invertedIndex = buildInvertedIndex(documents);

    // Print the inverted index
   // printInvertedIndex(invertedIndex);

    return invertedIndex;
}

bool BoolInver(std::string filepath) {
    std::unordered_map<std::string, std::vector<int>> InvertIn = CretaeInvertedIndex(filepath);
    std::this_thread::sleep_for(std::chrono::seconds(3));
    printInvertedIndex(InvertIn);
    return true;
}