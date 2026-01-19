#include "peer_node.h"
#include "ipc_server.h"
#include "socket_utils.h"
#include "logger.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

int main(int argc, char* argv[]) {
    if (!SocketUtils::init()) return 1;

    // Default configuration (or from argv)
    // Daemon usually runs on fixed ports or from config
    // For simplicity: Daemon runs on 9000 by default for P2P? 
    // Or we let the first 'tracker' command configure it?
    // Let's assume we start the daemon with: ./peer_daemon <MyP2PPort> <ControlPort>
    
    if (argc < 3) {
        std::cout << "Usage: peer_daemon <P2P_PORT> <CONTROL_PORT>" << std::endl;
        return 1;
    }
    
    int p2pPort = std::stoi(argv[1]);
    int controlPort = std::stoi(argv[2]);
    
    // Auto-calculate control port if not fixed? 
    // Simple: Fixed 9999 for single instance.
    
    // Default Tracker
    std::string tIp = "127.0.0.1";
    int tPort = 8080;
    
    Logger::log("Starting Peer Daemon...");
    PeerNode node(tIp, tPort, p2pPort);
    node.start();
    
    IPCServer ipc(controlPort, &node);
    ipc.start();
    
    Logger::log("Daemon is running. Use 'peer_cli' or 'tui.sh' to control.");
    
    // Keep alive
    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    SocketUtils::cleanup();
    return 0;
}
