#ifndef PEER_NODE_H
#define PEER_NODE_H

#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <map>
#include <atomic>
#include "socket_utils.h"
#include "protocol.h"

struct ChunkInfo {
    uint32_t index;
    std::string hash;
    bool present;
};

struct FileMetadata {
    std::string fileName;
    uint64_t fileSize;
    std::string fileHash;
    std::vector<std::string> chunkHashes; // NEW: Store per-chunk hashes
    std::string fullPath;
};

struct PeerConnection {
    std::string ip;
    uint16_t port;
};

class PeerNode {
public:
    PeerNode(const std::string& trackerIp, int trackerPort, int myPort);
    ~PeerNode();

    void start();
    void seedFile(const std::string& filepath);
    void downloadFile(const std::string& fileHash, const std::string& outputName);
    
    // TUI Support
    void setTracker(const std::string& ip, int port);

private:
    void serverLoop(); 
    void keepAliveLoop();

    // Tracker Ops
    void registerToTracker();
    void advertiseFile(const std::string& hash, uint64_t size, const std::string& name);
    std::vector<PeerConnection> getPeersForFile(const std::string& hash);

    // File Ops
    void splitFileBuffered(const std::string& filepath, FileMetadata& meta); 
    bool loadChunk(const FileMetadata& meta, uint32_t index, std::vector<char>& buffer);
    void writeChunk(const std::string& outputName, uint32_t index, const std::vector<char>& data);
    
    // Helper
    std::vector<std::string> fetchMetadata(const PeerConnection& peer, const std::string& fileHash, uint32_t chunkCount);

    std::string trackerIp;
    int trackerPort;
    int myPort;
    SocketType serverSocket;
    
    std::atomic<bool> running;
    std::thread serverThread;

    std::mutex dataMutex;
    std::map<std::string, FileMetadata> knownFiles; // Hash -> Metadata
};

#endif // PEER_NODE_H
