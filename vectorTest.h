#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <filesystem>
#include <filesystem>

std::unordered_map<std::string, std::vector<std::string>> readDocumentsFromFile(std::vector<std::string> filenames) {
    std::unordered_map<std::string, std::vector<std::string>> documents;
    for (const auto& filename : filenames) {
        std::ifstream file(filename);

        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return documents;
        }
        std::string line;
        while (std::getline(file, line)) {
            documents[filename].push_back(line);
        }

        file.close();
    }
    return documents;
}

std::unordered_map<std::string, std::vector<std::string>> CreateVector(std::string folderPath, int numOfThread, int delimeter) {

    std::vector<std::string> documents;
    int prefix = 750;
    int suffix = 1000;
    if (delimeter != 1) {
        prefix = 750 + numOfThread * (250 / delimeter);
        suffix = 750 + (numOfThread + 1) * (250 / delimeter) - 1;
        if (numOfThread == delimeter - 1) {
            suffix += 1;
            if (delimeter % 2) {
                suffix += 1;
            }
        }
    }
    if (folderPath == "datasets\\aclImdb\\aclImdb\\train\\unsup") {
        prefix = 3000 + numOfThread * (1000 / delimeter);
        suffix = 3000 + (numOfThread + 1) * (1000 / delimeter) - 1;
        if (numOfThread == delimeter - 1) {
            suffix += 1;
            if (delimeter % 2) {
                suffix += 1;
            }
        }
    }
    std::cout << "from vector ===" << prefix << "===" << suffix <<"\n";
    for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
        std::string filename = entry.path().filename().string();
        size_t pos = filename.find_first_of("_");
        int file_index;
        if (pos != std::string::npos) {
            file_index = std::stoi(filename.substr(0, pos));
        }
        if (file_index >= prefix && file_index <= suffix)
        {
            documents.push_back(entry.path().string());
        }
    }
    if (documents.empty()) {
        std::cerr << "No documents was found." << std::endl;
    }
    std::cout << documents.size() << "size\n";
    std::unordered_map<std::string, std::vector<std::string>> ReadVector = readDocumentsFromFile(documents);
    return ReadVector;
}
// "C:\\Users\\Владелец\\Desktop\\курсова\\InvertedIndex\\datasets\\aclImdb\\test\\neg";
//aclImdb\train\unsup
//    aclImdb\train\pos
//    aclImdb\train\neg
//    aclImdb\test\pos