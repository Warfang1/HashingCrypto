#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <sodium.h>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>

int main() {
    // Initialize libsodium
    if (sodium_init() < 0) {
        std::cout << "libsodium failed!" << std::endl;
        return 1;
    }

    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        std::cout << "Winsock failed!" << std::endl;
        return 1;
    }

    // Create socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cout << "Socket creation failed!" << std::endl;
        return 1;
    }

    sockaddr_in serverAddr;
serverAddr.sin_family = AF_INET;
serverAddr.sin_port = htons(8080);
inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
    std::cout << "Connection failed!" << std::endl;
    return 1;
}

std::cout << "Connected to server!" << std::endl;

char password[128];
std::cout << "Enter password: " << std::endl;
std::cin >> password;
send(clientSocket, password, strlen(password), 0);
std::cout << "Password sent!" << std::endl;

char response[20] = {0};
recv(clientSocket, response, sizeof(response), 0);
std::cout << "Server says: " << response << std::endl;

closesocket(clientSocket);
WSACleanup();


    return 0;
}