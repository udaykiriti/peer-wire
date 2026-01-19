# System Architecture

## Overview
PeerWire typically operates with one Tracker server and multiple Peer clients. The architecture handles peer discovery and direct peer-to-peer data transfer.

## Components

### 1. Tracker Server
The Tracker is a lightweight server that maintains a registry of which peers possess which files.
- **Responsibilities**:
    - Listen for Peer registrations.
    - Store mapping of File Hash -> List of Peers.
    - Respond to peer queries with a list of available peers and file metadata (size).
- **State**:
    - In-memory map protected by mutexes.
    - No persistent database (for this implementation).

### 2. Peer Client
The Peer acts as both a client and a server.
- **Seeder Mode**: 
    - Has the complete file.
    - Calculates SHA-256 hash.
    - Advertises file existence to the Tracker.
    - Listens for connection requests from other peers to upload chunks.
- **Leecher (Downloader) Mode**:
    - Queries Tracker for peers hosting a specific file hash.
    - Connects to multiple peers simultaneously.
    - Requests missing chunks in parallel.
    - Assembles the file locally.

## Data Flow

1. **Initialization**: Peer A (Seeder) starts, hashes file, registers with Tracker.
2. **Discovery**: Peer B (Leecher) asks Tracker for peers with File Hash X. 
3. **Response**: Tracker returns IP/Port of Peer A and the Total File Size.
4. **Transfer**: 
    - Peer B calculates number of chunks needed.
    - Peer B spawns worker threads.
    - Workers connect to Peer A (and any others) and request Chunk N.
    - Peer A sends Chunk N data.
    - Peer B writes data to disk.
