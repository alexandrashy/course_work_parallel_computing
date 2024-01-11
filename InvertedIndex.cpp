#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <iterator>
#include "vectorTest.h"
#include "Header.h"
#include "ThreadPool.h"

int main() {
    //std::unordered_map<std::string, std::vector<int>> InvertIn = CretaeInvertedIndex("C:\\Users\\Владелец\\Desktop\\курсова\\InvertedIndex\\datasets\\aclImdb\\test\\neg");
    //printInvertedIndex(InvertIn);
    std::string filepath = "C:\\Users\\Владелец\\Desktop\\курсова\\InvertedIndex\\datasets\\aclImdb\\test\\neg";
    thread_pool NewPool;
    std::cout << "Created";
    // "C:\\Users\\Владелец\\Desktop\\курсова\\InvertedIndex\\datasets\\aclImdb\\test\\neg";
    //aclImdb\train\unsup
    //    aclImdb\train\pos
    //    aclImdb\train\neg
    //    aclImdb\test\pos
    bool isSub = NewPool.submit(filepath);
    NewPool.submit("C:\\Users\\Владелец\\Desktop\\курсова\\InvertedIndex\\datasets\\aclImdb\\train\\unsup");
    NewPool.submit("C:\\Users\\Владелец\\Desktop\\курсова\\InvertedIndex\\datasets\\aclImdb\\train\\pos");
    NewPool.submit("C:\\Users\\Владелец\\Desktop\\курсова\\InvertedIndex\\datasets\\aclImdb\\train\\neg");

    std::cout << isSub;
    return 0;
}
