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
#include "Header.h"
#include "ThreadPool.h"
#pragma comment(lib, "ws2_32.lib") 


std::string handleClient(SOCKET clientSocket) {
    // Receive the message from the client
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    size_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

    if (bytesRead == -1) {
        std::cerr << "Error receiving message from client" << std::endl;
        return "0";
    }
    else if (bytesRead == 0) {
        std::cout << "Client disconnected" << std::endl;
        return "0";
    }
    else {
        std::string receivedMessage(buffer, bytesRead);
        std::cout << "Received message from client: " << receivedMessage << std::endl;
        return receivedMessage;
    }
    return "0";
}

int HandleClientsMap(std::unordered_map<SOCKET, std::string>& ClientMap) {
    return 1;
}


// Function to generate a random message
std::string generateRandomMessage() {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const int charsetSize = sizeof(charset) - 1;
    const int messageLength = 10; // Adjust the length as needed

    srand(static_cast<unsigned>(time(NULL)));

    std::string randomMessage;
    for (int i = 0; i < messageLength; ++i) {
        randomMessage += charset[rand() % charsetSize];
    }

    return randomMessage;
}

int main() {
    thread_pool NewPool;
    std::unordered_map<SOCKET, std::string> ClientMap;
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
        std::cerr << "Error binding socket" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Error listening for connections" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "Server listening on port 8080..." << std::endl;
    std::thread clientThread();
    // Accept a connection from a client
    while(true){
        sockaddr_in clientAddress;
        int clientAddressLength = sizeof(clientAddress);
        SOCKET clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Error accepting connection" << std::endl;
            closesocket(serverSocket);
            WSACleanup();
            return -1;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        std::cout << "Client connected" << std::endl;
        std::string message = generateRandomMessage();

        if (send(clientSocket, message.c_str(), strlen(message.c_str()), 0) == SOCKET_ERROR) {
            std::cerr << "Error sending message" << std::endl;
        }
        std::string ReceivedPath = handleClient(clientSocket);
        ClientMap[clientSocket] = ReceivedPath;
        bool isSub = NewPool.submit(ReceivedPath);
        std::cout << "Received Path - " << ReceivedPath << isSub << "\n";

    }
    for (const auto& entry : ClientMap) {
        closesocket(entry.first);
    }
    //closesocket(clientSocket);
    closesocket(serverSocket);

    
    WSACleanup();

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
//    //std::unordered_map<std::string, std::vector<int>> InvertIn = CretaeInvertedIndex("C:\\Users\\Владелец\\Desktop\\курсова\\InvertedIndex\\datasets\\aclImdb\\test\\neg");
//    //printInvertedIndex(InvertIn);
//    std::string filepath = "C:\\Users\\Владелец\\Desktop\\курсова\\InvertedIndex\\datasets\\aclImdb\\test\\neg";
//    thread_pool NewPool;
//    std::cout << "Created";
//    // "C:\\Users\\Владелец\\Desktop\\курсова\\InvertedIndex\\datasets\\aclImdb\\test\\neg";
//    //aclImdb\train\unsup
//    //    aclImdb\train\pos
//    //    aclImdb\train\neg
//    //    aclImdb\test\pos
//    bool isSub = NewPool.submit(filepath);
//    NewPool.submit("C:\\Users\\Владелец\\Desktop\\курсова\\InvertedIndex\\datasets\\aclImdb\\train\\unsup");
//    NewPool.submit("C:\\Users\\Владелец\\Desktop\\курсова\\InvertedIndex\\datasets\\aclImdb\\train\\pos");
//    NewPool.submit("C:\\Users\\Владелец\\Desktop\\курсова\\InvertedIndex\\datasets\\aclImdb\\train\\neg");
//
//    std::cout << isSub;
//    return 0;
//}
