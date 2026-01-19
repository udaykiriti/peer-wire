# PeerWire Project Tasks

- [x] Project Initialization
    - [x] Create `implementation_plan.md` <!-- id: 0 -->
    - [x] Setup CMake project structure <!-- id: 1 -->
- [x] Core Libraries & Utilities
    - [x] Implement Logging util <!-- id: 2 -->
    - [x] Implement Hashing util (SHA-256) <!-- id: 3 -->
    - [x] Implement Socket/Networking wrapper <!-- id: 4 -->
- [x] Tracker Server
    - [x] Design Tracker Protocol <!-- id: 5 -->
    - [x] Implement Tracker Server logic (Registration, Discovery) <!-- id: 6 -->
- [x] Peer Client
    - [x] Implement File Chunking & Metadata management <!-- id: 7 -->
    - [x] Implement Peer-to-Tracker communication <!-- id: 8 -->
    - [x] Implement Peer-to-Peer Server (Upload) <!-- id: 9 -->
    - [x] Implement Peer-to-Peer Client (Download) <!-- id: 10 -->
    - [x] Implement Multithreaded Transfer Manager <!-- id: 11 -->
- [x] Verification & Testing
    - [x] Unit tests for Chunks/Hashing <!-- id: 12 -->
    - [x] Integration test: Tracker + 2 Peers <!-- id: 13 -->
    - [x] Create `walkthrough.md` <!-- id: 14 -->
- [x] Documentation & Polish
    - [x] Create README.md (No Emojis) <!-- id: 15 -->
    - [x] Create Makefile <!-- id: 16 -->
- [x] Documentation & Polish
    - [x] Create README.md (No Emojis) <!-- id: 15 -->
    - [x] Create Makefile <!-- id: 16 -->
    - [x] Create `docs/` folder with details <!-- id: 17 -->

# Phase 2: Enhancements
- [x] Peer Heartbeats (Cleanup dead peers)
    - [x] Update Tracker to track `last_seen` <!-- id: 18 -->
    - [x] Implement Tracker cleanup thread <!-- id: 19 -->
    - [x] Implement Peer heartbeat sender <!-- id: 20 -->
- [x] Chunk Integrity
    - [x] Add chunk hashes to Tracker response <!-- id: 21 -->
    - [x] Verify individual chunks on receipt <!-- id: 22 -->
- [x] UX Improvements
    - [x] Add CLI Progress Bar <!-- id: 23 -->

# Phase 3: Interactive TUI
- [x] Implement Shell Loop <!-- id: 24 -->
    - [x] Create `Shell` class to handle commands <!-- id: 25 -->
    - [x] Refactor `peer_main.cpp` to use Shell <!-- id: 26 -->
    - [x] Implement Shell Loop <!-- id: 24 -->
    - [x] Create `Shell` class to handle commands <!-- id: 25 -->
    - [x] Refactor `peer_main.cpp` to use Shell <!-- id: 26 -->
    - [x] Add `connect`, `seed`, `download`, `help` commands <!-- id: 27 -->

# Phase 4: Final Polish
- [x] Create `.gitignore` <!-- id: 28 -->
- [x] Create helper `.sh` scripts <!-- id: 29 -->
- [x] Create helper `.sh` scripts <!-- id: 29 -->
- [x] Update Makefile headers <!-- id: 30 -->

# Phase 5: Polish & UI
- [x] Add ANSI Colors support <!-- id: 31 -->
- [x] Improve Shell Prompts & Banners <!-- id: 32 -->
- [x] Improve Shell Prompts & Banners <!-- id: 32 -->
- [x] Improve `help` command formatting <!-- id: 33 -->

# Phase 6: Architecture Refactor (Client-Server)
- [/] Reorganize Folder Structure (`src/node`, `src/ipc`) <!-- id: 34 -->
- [ ] Implement C++ IPC Server (Control Socket) <!-- id: 35 -->
- [ ] Create `ipc_client` helper utility <!-- id: 36 -->
- [ ] Build `tui.sh` (Shell-based frontend) <!-- id: 37 -->
