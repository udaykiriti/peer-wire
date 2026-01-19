#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <cstdint>

enum class PacketType : uint8_t {
    REGISTER = 1,
    KEEP_ALIVE = 2,
    REQUEST_PEERS = 3,
    FILE_INFO = 4,    // Sent by peer to tracker to say "I have this file" (Simplification: merged with REGISTER usually, but let's keep it explicit if needed)
                      // Actually, let's say REGISTER just registers the peer, and FILE_INFO adds a file.
                      // Or REGISTER includes file info. Let's make REGISTER simple (Peer -> Tracker: Port)
                      // And a separate ADVERTISE_FILE (Peer -> Tracker: FileHash)
    
    ADVERTISE_FILE = 5,
    
    // Peer <-> Peer
    REQUEST_METADATA = 30,
    RESPONSE_METADATA = 31,
    
    REQUEST_CHUNK = 10,
    SEND_CHUNK = 11,
    
    // Responses
    RESPONSE_PEERS = 20, // Tracker -> Peer: List of IPs/Ports
    RESPONSE_OK = 21,
    RESPONSE_ERROR = 22
};

#pragma pack(push, 1)
struct PacketHeader {
    uint32_t length; // Body length
    PacketType type;
};
#pragma pack(pop)

// Example Payload Structures (Serialize manually or using structs)

// Register: 
// [Header] [Port (uint16_t)]

// Advertise File:
// [Header] [FileHash (32 bytes)] [FileSize (uint64_t)] [FileNameLen(uint32_t)] [FileName...]

// Request Peers:
// [Header] [FileHash (32 bytes)]

// Response Peers:
// [Header] [Count (uint32_t)] [Peer1 IP(16b)][Peer1 Port(u16)] ... 

// Request Chunk:
// [Header] [FileHash (32 bytes)] [ChunkIndex (uint32_t)]

// Send Chunk:
// [Header] [FileHash (32 bytes)] [ChunkIndex (uint32_t)] [DataSize (uint32_t)] [Data...]

#endif // PROTOCOL_H
