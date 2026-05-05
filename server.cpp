#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <sodium.h>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <fstream>

int main() {
    if (sodium_init() < 0) {
        std::cout << "libsodium failed!" << std::endl;
        return 1;
    }

    WSADATA wsaData;
if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
    std::cout << "Winsock failed!" << std::endl;
    return 1;
}

SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
if (serverSocket == INVALID_SOCKET) {
    std::cout << "Socket creation failed!" << std::endl;
    return 1;
}

sockaddr_in serverAddr;
serverAddr.sin_family = AF_INET;
serverAddr.sin_addr.s_addr = INADDR_ANY;
serverAddr.sin_port = htons(8080);

if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
    std::cout << "Bind failed!" << std::endl;
    return 1;
}

std::cout << "Server started on port 8080!" << std::endl;

listen(serverSocket, 1);
std::cout << "Waiting for client..." << std::endl;

SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
if (clientSocket == INVALID_SOCKET) {
    std::cout << "Accept failed!" << std::endl;
    return 1;
}

std::cout << "Client connected!" << std::endl;

char password[128] = {0};
recv(clientSocket, password, sizeof(password), 0);
std::cout << "Received password: " << password << std::endl;

char hashed_password[crypto_pwhash_STRBYTES];

std::ifstream file("hash.txt");
if (file.good()) {
    // Load hash from file and verify
    file >> hashed_password;
    file.close();
    if (crypto_pwhash_str_verify(hashed_password, password, strlen(password)) == 0) {
        std::cout << "Password matches!" << std::endl;
        send(clientSocket, "OK", 2, 0);
    } else {
        std::cout << "Wrong password!" << std::endl;
        send(clientSocket, "NO", 2, 0);
    }
} else {
    // Hash and save to file
    if (crypto_pwhash_str(hashed_password, password, strlen(password), crypto_pwhash_OPSLIMIT_INTERACTIVE, crypto_pwhash_MEMLIMIT_INTERACTIVE) != 0) {
        std::cout << "Hashing failed!" << std::endl;
        return 1;
    }
    std::ofstream outFile("hash.txt");
    outFile << hashed_password;
    outFile.close();
    std::cout << "Password registered and hash saved!" << std::endl;
    send(clientSocket, "Registered!", 11, 0);
}

closesocket(clientSocket);
closesocket(serverSocket);
WSACleanup();
return 0;
}