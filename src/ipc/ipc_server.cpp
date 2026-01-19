#include "ipc_server.h"
#include "socket_utils.h"
#include "logger.h"
#include "colors.h"
#include <thread>
#include <sstream>
#include <vector>

IPCServer::IPCServer(int port, PeerNode* node) : port(port), node(node) {}

void IPCServer::start() {
    std::thread(&IPCServer::serverLoop, this).detach();
    Logger::log("IPC Server listening on local port " + std::to_string(port));
}

void IPCServer::serverLoop() {
    SocketType listener = SocketUtils::createSocket();
    if (!SocketUtils::bindSocket(listener, port)) {
        Logger::error("Failed to bind IPC socket");
        return;
    }
    SocketUtils::listenSocket(listener);

    while (true) {
        std::string clientIp; // Should be 127.0.0.1
        SocketType client = SocketUtils::acceptConnection(listener, clientIp);
        if (client == INVALID_SOCKET) continue;

        // One-shot: Read command, send response, close.
        uint32_t len = 0;
        if (SocketUtils::recvAll(client, &len, sizeof(len))) {
            std::vector<char> buf(len + 1);
            if (SocketUtils::recvAll(client, buf.data(), len)) {
                buf[len] = '\0';
                std::string cmd(buf.data());
                
                std::string response = handleCommand(cmd);
                
                uint32_t respLen = (uint32_t)response.size();
                SocketUtils::sendAll(client, &respLen, sizeof(respLen));
                SocketUtils::sendAll(client, response.c_str(), respLen);
            }
        }
        SocketUtils::closeSocket(client);
    }
}

std::string IPCServer::handleCommand(const std::string& cmd) {
    std::stringstream ss(cmd);
    std::string action;
    ss >> action;
    
    if (action == "seed") {
        std::string path;
        ss >> path;
        if (path.empty()) return Color::RED + "Usage: seed <path>" + Color::RESET;
        
        // This runs in IPC thread, might need thread-safety in node?
        // Node implementation uses mutexes, so it should be fine.
        node->seedFile(path);
        return Color::GREEN + "Started seeding: " + path + Color::RESET;
    }
    else if (action == "download") {
        std::string hash, out;
        ss >> hash >> out;
        if (hash.empty() || out.empty()) return Color::RED + "Usage: download <hash> <out>" + Color::RESET;
        
        node->downloadFile(hash, out);
        return Color::GREEN + "Started download for " + hash + Color::RESET;
    }
    else if (action == "tracker") {
        std::string ip; 
        int p;
        ss >> ip >> p;
        node->setTracker(ip, p);
        return "Tracker updated.";
    }
    else if (action == "ping") {
        return "pong";
    }
    
    return Color::RED + "Unknown command" + Color::RESET;
}
