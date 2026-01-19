# Phase 6: Client-Server Architecture & Refactoring

## Goal
Separate the application into a **Daemon** (C++ Backend) and a **Client** (Shell TUI), communicating via a local TCP socket (IPC). Improve folder structure.

## New Folder Structure
```
src/
  common/       # Utils (Log, Hash, Socket, Color)
  protocol/     # Packet definitions
  tracker/      # Tracker Server Code
  node/         # Peer Logic (P2P, FileMgmt) - Was src/peer
  ipc/          # [NEW] Control Socket Server
  tools/        # [NEW] Helper tools (ipc_send)
```

## Architecture
1.  **Peer Daemon (`bin/peer_daemon`)**:
    - Runs the `PeerNode`.
    - Starts an `IPCServer` on Localhost Port `9999` (or `MyPort + 100`).
    - Listens for text commands: `seed <file>`, `download <hash>`.
    - Returns JSON/Text responses.

2.  **IPC Helper (`bin/send_cmd`)**:
    - Simple C++ tool: `./send_cmd <PORT> <MSG>`
    - Connects to localhost, sends msg, prints response, exits.

3.  **Shell TUI (`ui.sh`)**:
    - A bash script loop.
    - Draws a menu.
    - Calls `send_cmd` to talk to daemon.

## Implementation Steps
1.  **Restructure**: Request `fd` moves. Update `CMakeLists.txt`.
2.  **IPC Server**: Create `src/ipc/ipc_server.h/cpp`.
    - Accept loop (separate thread).
    - Parse simple string commands.
    - Call `PeerNode` methods.
3.  **IPC Client**: Create `src/tools/send_cmd.cpp`.
4.  **TUI Script**: Write `tui.sh` with a nice loop.
