#include "socket_utils.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: ipc_client <PORT> <COMMAND...>" << std::endl;
        return 1;
    }

    if (!SocketUtils::init()) return 1;

    int port = std::stoi(argv[1]);
    std::string command;
    for (int i = 2; i < argc; ++i) {
        command += argv[i];
        if (i < argc - 1) command += " ";
    }

    SocketType sock = SocketUtils::createSocket();
    if (!SocketUtils::connectToServer(sock, "127.0.0.1", port)) {
        std::cerr << "Error: Could not connect to Peer Daemon on port " << port << std::endl;
        SocketUtils::closeSocket(sock);
        SocketUtils::cleanup();
        return 1;
    }

    // Send command
    uint32_t len = (uint32_t)command.size();
    SocketUtils::sendAll(sock, &len, sizeof(len));
    SocketUtils::sendAll(sock, command.c_str(), len);

    // Receive response
    uint32_t respLen = 0;
    if (SocketUtils::recvAll(sock, &respLen, sizeof(respLen))) {
        std::vector<char> buf(respLen + 1);
        if (SocketUtils::recvAll(sock, buf.data(), respLen)) {
            buf[respLen] = '\0';
            std::cout << buf.data() << std::endl;
        }
    }

    SocketUtils::closeSocket(sock);
    SocketUtils::cleanup();
    return 0;
}
