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
#include "UserDataStructure.h"
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
        {"sasha", "datasets\\aclimdb\\aclimdb\\test\\neg"},
        {"alex", "datasets\\aclimdb\\aclimdb\\test\\pos"},
        {"oleksadnra", "datasets\\aclimdb\\aclimdb\\train\\neg"},
        {"alexandra", "datasets\\aclimdb\\aclimdb\\train\\pos"},
        {"alexandra", "datasets\\aclimdb\\aclimdb\\train\\unsup"}
    };
    //for (const auto& User: Users) {
    //    NewPool.submit(User);
    //}
    for (int i = 0; i <= 10; i++) {
        std::string USER_NAME = generateRandomMessage();
        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> distribution(0, 4);
        int random_number = distribution(rng);
        USER_DATA User(users[random_number].Path, USER_NAME);
        NewPool.submit(User);
    }
}

int consuqence(){



}



int main() {

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
//       "C:\\Users\\Владелец\\Desktop\\курсова\\InvertedIndex\\datasets\\aclImdb\\train\\unsup"
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
