#include "socket_utils.h"
#include "logger.h"
#include <iostream>

bool SocketUtils::init() {
#ifdef _WIN32
    WSADATA wsaData;
    int res = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (res != 0) {
        Logger::error("WSAStartup failed: " + std::to_string(res));
        return false;
    }
#endif
    return true;
}

void SocketUtils::cleanup() {
#ifdef _WIN32
    WSACleanup();
#endif
}

SocketType SocketUtils::createSocket() {
    SocketType sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        Logger::error("Failed to create socket");
    }
    return sock;
}

bool SocketUtils::bindSocket(SocketType sock, int port) {
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        Logger::error("Bind failed on port " + std::to_string(port));
        return false;
    }
    return true;
}

bool SocketUtils::listenSocket(SocketType sock) {
    if (listen(sock, SOMAXCONN) == SOCKET_ERROR) {
        Logger::error("Listen failed");
        return false;
    }
    return true;
}

SocketType SocketUtils::acceptConnection(SocketType sock, std::string& clientIp) {
    sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);
#ifdef _WIN32
    SocketType clientSock = accept(sock, (struct sockaddr*)&clientAddr, &clientAddrLen);
#else
    SocketType clientSock = accept(sock, (struct sockaddr*)&clientAddr, (socklen_t*)&clientAddrLen);
#endif

    if (clientSock == INVALID_SOCKET) {
        Logger::error("Accept failed");
        return INVALID_SOCKET;
    }

    char ipStr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &clientAddr.sin_addr, ipStr, INET_ADDRSTRLEN);
    clientIp = ipStr;
    return clientSock;
}

bool SocketUtils::connectToServer(SocketType sock, const std::string& ip, int port) {
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);

    if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        // Logger::error("Connect failed to " + ip + ":" + std::to_string(port));
        return false;
    }
    return true;
}

void SocketUtils::closeSocket(SocketType sock) {
#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif
}

bool SocketUtils::sendAll(SocketType sock, const void* data, size_t size) {
    const char* ptr = static_cast<const char*>(data);
    size_t totalSent = 0;
    while (totalSent < size) {
        int sent = send(sock, ptr + totalSent, size - totalSent, 0);
        if (sent == SOCKET_ERROR) {
            return false;
        }
        totalSent += sent;
    }
    return true;
}

bool SocketUtils::recvAll(SocketType sock, void* data, size_t size) {
    char* ptr = static_cast<char*>(data);
    size_t totalReceived = 0;
    while (totalReceived < size) {
        int received = recv(sock, ptr + totalReceived, size - totalReceived, 0);
        if (received == SOCKET_ERROR || received == 0) {
            return false;
        }
        totalReceived += received;
    }
    return true;
}
