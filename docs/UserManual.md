# PeerWire User Manual

Welcome to **PeerWire**, a robust P2P file sharing system. This manual guides you through installation, usage, and troubleshooting.

## 1. Quick Start
### Prerequisites
- Windows or Linux
- CMake 3.10+
- C++17 Compiler

### Installation
Run the build script:
```bash
scripts/build.sh
```

## 2. Running the Application
PeerWire uses a **Client-Server** architecture.

### Step 1: Start the Tracker
The tracker acts as the directory server.
```bash
scripts/run_tracker.sh
```
*Default: Listens on port 8080.*

### Step 2: Start the Daemon
The daemon runs in the background and handles file transfers.
```bash
scripts/run_daemon.sh <P2P_PORT> <CONTROL_PORT>
```
Example:
```bash
scripts/run_daemon.sh 9001 9991
```

### Step 3: Connect with TUI
The TUI (Text User Interface) sends commands to your daemon.
```bash
scripts/tui.sh
```
*Note: The script defaults to connecting to control port 9999. Verify the script matches your daemon.*

## 3. Commands
Inside the TUI `PeerWire>` prompt:

| Command | Description | Example |
| :--- | :--- | :--- |
| `tracker <ip> <port>` | Set tracker address | `tracker 127.0.0.1 8080` |
| `seed <file>` | Seed a file to the network | `seed my_video.mp4` |
| `download <hash> <out>` | Download a file by hash | `download a1b2... output.mp4` |
| `exit` | Exit the TUI (Daemon stays running) | `exit` |

## 4. Troubleshooting
- **Binding Failed**: Ensure ports are not in use.
- **Connection Refused**: Ensure Tracker is running before starting Daemon.
- **Firewall**: Allow the application through your firewall.

## 5. Advanced
See [Architecture](design_refactor.md) for technical details.
