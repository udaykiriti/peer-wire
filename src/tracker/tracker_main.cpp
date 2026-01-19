#include "socket_utils.h"
#include "logger.h"
#include "protocol.h"
#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include <algorithm>
#include <cstring>

struct PeerInfo {
    std::string ip;
    uint16_t port;
    time_t lastSeen;

    bool operator==(const PeerInfo& other) const {
        return ip == other.ip && port == other.port;
    }
};

struct FileRegistryEntry {
    uint64_t size;
    std::vector<PeerInfo> peers;
};

// Global state
std::mutex stateMutex;
// FileHash -> Entry
std::map<std::string, FileRegistryEntry> registry;

void cleanupLoop() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
        
        std::lock_guard<std::mutex> lock(stateMutex);
        time_t now = std::time(nullptr);
        
        for (auto& [hash, entry] : registry) {
            auto& peers = entry.peers;
            for (auto it = peers.begin(); it != peers.end(); ) {
                if (now - it->lastSeen > 60) {
                    Logger::log("Removing dead peer " + it->ip + ":" + std::to_string(it->port));
                    it = peers.erase(it);
                } else {
                    ++it;
                }
            }
        }
    }
}

void handleClientWithSession(SocketType clientSock, std::string clientIp) {
    Logger::log("New connection from " + clientIp);
    uint16_t peerPort = 0;

    while (true) {
        PacketHeader header;
        if (!SocketUtils::recvAll(clientSock, &header, sizeof(header))) break;

        if (header.type == PacketType::REGISTER) {
            if (!SocketUtils::recvAll(clientSock, &peerPort, sizeof(peerPort))) break;
            Logger::log("Peer " + clientIp + " declared listening port " + std::to_string(peerPort));
        }
        else if (header.type == PacketType::KEEP_ALIVE) {
             uint16_t pPort = 0;
             if (!SocketUtils::recvAll(clientSock, &pPort, sizeof(pPort))) break;
             
             // Update timestamp for this peer in all entries
             std::lock_guard<std::mutex> lock(stateMutex);
             PeerInfo target{clientIp, pPort, 0};
             time_t now = std::time(nullptr);
             
             int updatedCount = 0;
             for (auto& [hash, entry] : registry) {
                 for (auto& p : entry.peers) {
                     if (p == target) {
                         p.lastSeen = now;
                         updatedCount++;
                     }
                 }
             }
             if(updatedCount > 0) {
                // Logger::log("Heartbeat from " + clientIp); // Verbose
             }
        }
        else if (header.type == PacketType::ADVERTISE_FILE) {
            char rawHash[32];
            if (!SocketUtils::recvAll(clientSock, rawHash, 32)) break;
            
            uint64_t fSize;
            if (!SocketUtils::recvAll(clientSock, &fSize, sizeof(fSize))) break;
            
            uint32_t nLen;
            if (!SocketUtils::recvAll(clientSock, &nLen, sizeof(nLen))) break;
            
            std::vector<char> nBuf(nLen);
             if (!SocketUtils::recvAll(clientSock, nBuf.data(), nLen)) break;
            
            std::string hashStr;
            for(int i=0; i<32; i++) {
                char buf[3];
                sprintf(buf, "%02x", (unsigned char)rawHash[i]);
                hashStr += buf;
            }

            if (peerPort == 0) {
                 Logger::error("Peer tried to advertise without REGISTERing port first.");
                 continue; 
            }

            std::lock_guard<std::mutex> lock(stateMutex);
            PeerInfo p{clientIp, peerPort, std::time(nullptr)};
            
            auto& entry = registry[hashStr];
            entry.size = fSize; // Update size (assume consistent)
            
            bool found = false;
            for(auto& existing : entry.peers) {
                if(existing == p) { found = true; break;}
            }
            if(!found) entry.peers.push_back(p);
            
            Logger::log("Registered file " + hashStr + " (" + std::to_string(fSize) + " bytes) for peer " + clientIp);
        }
        else if (header.type == PacketType::REQUEST_PEERS) {
             char rawHash[32];
             if (!SocketUtils::recvAll(clientSock, rawHash, 32)) break;
             
             std::string hashStr;
             for(int i=0; i<32; i++) {
                char buf[3];
                sprintf(buf, "%02x", (unsigned char)rawHash[i]);
                hashStr += buf;
            }
             
             std::lock_guard<std::mutex> lock(stateMutex);
             std::vector<PeerInfo> peers;
             uint64_t fileSize = 0;
             if (registry.count(hashStr)) {
                 peers = registry[hashStr].peers;
                 fileSize = registry[hashStr].size;
             }
             
             // Response: [PacketType RESPONSE_PEERS] [FileSize u64] [Count u32] [IPLen][IP][Port]...
             // Updated protocol dynamically here (compatible because client assumes size is coming now)
             
             std::vector<uint8_t> payload;
             
             auto append = [&](const void* d, size_t s){
                 const uint8_t* b = (const uint8_t*)d;
                 payload.insert(payload.end(), b, b+s);
             };
             
             append(&fileSize, sizeof(fileSize));
             
             uint32_t count = (uint32_t)peers.size();
             append(&count, sizeof(count));

             for(const auto& p : peers) {
                 uint8_t ipLen = (uint8_t)p.ip.length();
                 append(&ipLen, 1);
                 append(p.ip.data(), ipLen);
                 append(&p.port, sizeof(p.port));
             }
             
             PacketHeader resp;
             resp.type = PacketType::RESPONSE_PEERS;
             resp.length = (uint32_t)payload.size();
             
             SocketUtils::sendAll(clientSock, &resp, sizeof(resp));
             SocketUtils::sendAll(clientSock, payload.data(), payload.size());
             
             Logger::log("Returned " + std::to_string(count) + " peers for " + hashStr);
        }
    }
    SocketUtils::closeSocket(clientSock);
}

int main() {
    if (!SocketUtils::init()) return 1;

    SocketType listener = SocketUtils::createSocket();
    if (listener == INVALID_SOCKET) return 1;

    if (!SocketUtils::bindSocket(listener, 8080)) return 1;
    if (!SocketUtils::listenSocket(listener)) return 1;

    Logger::log("Tracker started on port 8080");

    std::thread(cleanupLoop).detach();

    while (true) {
        std::string clientIp;
        SocketType client = SocketUtils::acceptConnection(listener, clientIp);
        if (client != INVALID_SOCKET) {
            std::thread(handleClientWithSession, client, clientIp).detach();
        }
    }

    SocketUtils::cleanup();
    return 0;
}
