# PeerWire Walkthrough

PeerWire is a distributed P2P file sharing system with a **Client-Server Architecture**.

## Architecture
- **Daemon (`peer_daemon`)**: C++ background service that handles P2P networking, seeding, and downloading.
- **Client (`tui.sh`)**: Shell-based TUI that sends commands to the daemon via a local IPC socket.

## Prerequisites
- CMake 3.10+
- C++17 Compiler
- Windows (Winsock) or Linux (POSIX)

## Build Instructions
```bash
scripts/build.sh
```

## Running the System

### 1. Start the Tracker
The tracker coordinates peers.
```bash
scripts/run_tracker.sh
```

### 2. Start the Daemon
Start the background service on a specific P2P port.
```bash
# Usage: scripts/run_daemon.sh <P2P_PORT> <CONTROL_PORT>
scripts/run_daemon.sh 9001 9991
```

### 3. Open the TUI (Client)
Connect and control the daemon.
```bash
# Connects to port 9999 by default (edit script for others)
scripts/tui.sh
```

## Running Tests
To run the automated integration test:
```bash
python scripts/integration_test.py
```

## Folder Structure
- `scripts/`: Helper shell and python scripts.
- `src/tracker`: Tracker Server code.
- `src/node`: P2P Logic (Seeding/Downloading).
- `src/ipc`: IPC Server (Control Socket).
- `src/tools`: IPC Client (`send_cmd`).
- `src/common`: Shared utilities.
