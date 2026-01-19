#ifndef IPC_SERVER_H
#define IPC_SERVER_H

#include <string>
#include <functional>
#include "peer_node.h"

// Callback: Command string -> Response string
using CommandHandler = std::function<std::string(const std::string&)>;

class IPCServer {
public:
    IPCServer(int port, PeerNode* node);
    void start(); // Starts in a detached thread

private:
    int port;
    PeerNode* node;
    void serverLoop();
    std::string handleCommand(const std::string& cmd);
};

#endif // IPC_SERVER_H
