#pragma once
#include <iostream>
#include <cstring>
#include <winsock2.h>
#include<thread>
#include<ctime>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <iterator>
#include "vectorTest.h"
#include "ThreadPool.h"
#include "UserDataStructure.h"
#include <filesystem>
#pragma comment(lib, "ws2_32.lib") 


USER_DATA handleClient(SOCKET clientSocket) {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    size_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

    if (bytesRead == -1) {
        std::cerr << "Error receiving message from client" << std::endl;
        
    }
    else if (bytesRead == 0) {
        std::cout << "Client disconnected" << std::endl;
        
    }
    else {
        std::string receivedMessage(buffer, bytesRead);
        std::cout << "Received message from client: " << receivedMessage << std::endl;
        std::stringstream ss(receivedMessage);
        
        USER_DATA user;
        std::string word;
        std::vector<std::string> words;

        while (ss >> word) {
            words.push_back(word);
        }
        user.Path = words[1];
        user.USER_NAME = words[0];

        return user;
    }
}


// Function to generate a random message
std::string RandomStr(long int n) {
    srand(time(NULL));
    std::string StringRev;
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> distribution(97, 122);
    for (int i = 0; i < n; ++i) {
        char c = distribution(rng); // Generate a random character
        StringRev += c;
    }
    return StringRev;
}

bool SendClientData(SOCKET clientSocket, std::string& message) {
     if (send(clientSocket, message.c_str(), strlen(message.c_str()), 0) == SOCKET_ERROR) {
            std::cerr << "Error sending message" << std::endl;
            return true;
     }
     return false;
}


int server() {
    thread_pool NewPool;
    std::vector<std::string> clients;
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize/n";
        return -1;
    }

    // Create a socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Error creating socket/n";
        WSACleanup();
        return -1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8080);

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Error binding socket\n";
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Error listening for connections\n";
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "Server listening on port 8080...\n";
    // Accept a connection from a client
    while (true) {
        sockaddr_in clientAddress;
        int clientAddressLength = sizeof(clientAddress);
        SOCKET clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Error accepting connection\n";
            closesocket(serverSocket);
            WSACleanup();
            return -1;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        std::cout << "Client connected" << std::endl;
        USER_DATA ReceivedData = handleClient(clientSocket);
        std::string Message;
        if (std::find(clients.begin(), clients.end(), ReceivedData.USER_NAME) != clients.end()) {
            Message = "The user already exists, try another username";
        }
        else {
            if (NewPool.submit(ReceivedData)) {
                clients.push_back(ReceivedData.USER_NAME);
                Message = "Received Path - " + ReceivedData.Path + " added to execution\n";
            }
            else {
                Message = "The server are unavailable\n";
            }
        }
        SendClientData(clientSocket, Message);
        closesocket(clientSocket);
    }

    closesocket(serverSocket);

    WSACleanup();

    return 0;
}


int parallel() {
    //● aclImdb\test\neg – N = 12500 файлів;
    //● aclImdb\test\pos – N = 12500 файлів;
    //● aclImdb\train\neg – N = 12500 файлів;
    //● aclImdb\train\pos – N = 12500 файлів;
    //● aclImdb\train\unsup – N = 50000 файлів.
    thread_pool NewPool;
    std::vector<USER_DATA> users = {
        {"Cdatasets\\aclImdb\\aclImdb\\test\\neg", "sasha"},
        {"datasets\\aclimdb\\aclimdb\\test\\pos", "alex"},
        {"datasets\\aclimdb\\aclimdb\\train\\neg","oleksadnra"},
        {"datasets\\aclimdb\\aclimdb\\train\\pos", "alexandra"},
        {"datasets\\aclimdb\\aclimdb\\train\\unsup", "aleksandra"}
    };
    int count = 1;
    thread_count = 1; //td::thread::hardware_concurrency()
    child_threads_count = 2;
    auto payload_begin = high_resolution_clock::now();
    std::string UserName;
    std::vector<std::string> clients;
    for (int i = 0; i < count; i++) {
        USER_DATA User = users[i];
        //std::cout << User.Path << User.USER_NAME << "\n";
        if (std::find(clients.begin(), clients.end(), User.USER_NAME) != clients.end()) {
            std::cout << "The user already exists, try another username";
        }
        else {
            if (NewPool.submit(User)) {
                clients.push_back(UserName);
                std::cout << "Received Path - " + User.Path + " added to execution\n";
            }
            else {
                std::cout << "The server are unavailable\n";
            }
        }
    }
    //std::this_thread::sleep_for(std::chrono::seconds(50));
    std::filesystem::path DirectoryPath = "users_data\\";
    //std::cout << "here" << "\n";
    while (std::distance(std::filesystem::directory_iterator(DirectoryPath), std::filesystem::directory_iterator{}) != count*child_threads_count) {
        continue;
    }
    auto payload_end = high_resolution_clock::now();
    //std::this_thread::sleep_for(std::chrono::seconds(10));
    auto elapsed = duration_cast<nanoseconds>(payload_end - payload_begin);
    std::cout << "\nPayload Time: " << elapsed.count() * 1e-9 << " seconds.\n";
    return 0;
}



int main() {
    parallel();
    //server();
    return 0;
}





//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <unordered_map>
//#include <vector>
//#include <algorithm>
//#include <iterator>
//#include "vectorTest.h"
//#include "Header.h"
//#include "ThreadPool.h"

//int main() {
//    //std::unordered_map<std::string, std::vector<int>> InvertIn = CretaeInvertedIndex("InvertedIndex\\datasets\\aclImdb\\test\\neg");
//    //printInvertedIndex(InvertIn);
//    std::string filepath = "InvertedIndex\\datasets\\aclImdb\\test\\neg";
//    thread_pool NewPool;
//    std::cout << "Created";
//    // "InvertedIndex\\datasets\\aclImdb\\test\\neg";
//       "InvertedIndex\\datasets\\aclImdb\\train\\unsup"
//    //aclImdb\train\unsup
//    //    aclImdb\train\pos
//    //    aclImdb\train\neg
//    //    aclImdb\test\pos
//    bool isSub = NewPool.submit(filepath);
//    NewPool.submit("InvertedIndex\\datasets\\aclImdb\\train\\unsup");
//    NewPool.submit("datasets\\aclImdb\\train\\pos");
//    NewPool.submit("datasets\\aclImdb\\train\\neg");
//
//    std::cout << isSub;
//    return 0;
//}
