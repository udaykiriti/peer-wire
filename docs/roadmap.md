# Future Roadmap

Your PeerWire system is functional, but here are several ways to make it production-ready:

## 1. Robustness & Data Integrity
- [ ] **Chunk-Level Integrity**: Currently, we verify the hash of the *entire* file after download. If a single chunk is corrupt, the whole file is discarded. 
    - *Improvement*: Include a list of SHA-256 hashes (one per chunk) in the `RESPONSE_PEERS` or a new `FILE_METADATA` packet. Verify each chunk upon receipt.
- [ ] **Data Persistence**: Save the downloaded chunks to disk immediately. If the client crashes, check existing file size/hashes and only download missing parts (Resume capability).
- [ ] **Peer Heartbeats**: The Tracker thinks peers are online forever. Implement a `KEEP_ALIVE` packet periodically. If a peer doesn't ping for 60s, remove them from the registry.

## 2. Performance
- [ ] **Memory Mapping (mmap)**: Instead of `std::fstream` seek/write (which can be slow), map the target file into memory. This lets the OS handle paging and caching efficiently.
- [ ] **Asynchronous I/O**: The current "one thread per peer" model doesn't scale to thousands of connections. Use non-blocking sockets with `select`, `poll`, or `epoll` (Linux) / `IOCP` (Windows).

## 3. User Experience
- [ ] **CLI Progress Bar**: Show a visual progress bar `[=====>    ] 50%` with download speed metrics.
- [ ] **Interactive Shell**: Instead of running the command and exiting, allow the user to stay in a shell:
    ```text
    PeerWire> connect 127.0.0.1 8080
    PeerWire> search "movie"
    PeerWire> download <hash>
    ```

## 4. Networking
- [ ] **NAT Traversal**: Real P2P needs to work across routers. Implement **UPnP** or **STUN/hole-punching** so peers behind home routers can talk to each other.
