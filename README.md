# PeerWire

PeerWire is a distributed file sharing system implemented in C++. It eliminates reliance on a single server by allowing peers to both download and upload file chunks concurrently.

## Overview

The system follows a tracker-based architecture. Peers register themselves and discover other peers sharing the same file. Each file is divided into fixed-size chunks, and each chunk is verified using cryptographic hashes (SHA-256) to ensure data integrity.

## Features

- Decentralized file transfer (P2P)
- Tracker-based peer discovery
- Parallel chunk downloading
- Cryptographic data verification
- Cross-platform support (Windows/Linux)

## Build Instructions

### Prerequisites
- C++17 compliant compiler
- CMake 3.10 or higher

### Building
You can use the provided Makefile or scripts:
```bash
# Using Make
make

# Using Script
scripts/build.sh
```

The executables (tracker and peer) will be placed in `build/bin`.

## Usage

### Shortcuts (Scripts)
Convenience scripts are in the `scripts/` directory:
- `scripts/run_tracker.sh`: Starts the Tracker.
- `scripts/run_daemon.sh`: Starts the Daemon.
- `scripts/tui.sh`: Starts the TUI Client.


## Documentation
- [Detailed Walkthrough](docs/walkthrough.md)
- [Architecture & Design](docs/design_refactor.md)
- [Project Tasks](docs/tasks.md)

## Usage
See the `scripts/` directory for run scripts.


1. Enter your listening port (e.g., `9001`) when prompted.
2. Default tracker is `127.0.0.1:8080`. Use `tracker <IP> <Port>` to change it.
3. Use commands:
    - `seed <file>`
    - `download <hash> <outfile>`
    - `help`

### CLI Mode (Legacy)
#### 1. Start Support Server (Tracker)
The tracker coordinates peers.

```bash
./build/bin/tracker
```

### 2. Seeding a File
To share a file, start a peer in seed mode.

```bash
./build/bin/peer <MyPort> <TrackerIP> <TrackerPort> seed <FilePath>
```

Example:
```bash
./build/bin/peer 9001 127.0.0.1 8080 seed mydata.txt
```

### 3. Downloading a File
To download a file, start a peer in download mode using the file hash obtained from the seeder.

```bash
./build/bin/peer <MyPort> <TrackerIP> <TrackerPort> download <FileHash> <OutputFileName>
```

Example:
```bash
./build/bin/peer 9002 127.0.0.1 8080 download a1b2c3... downloaded_data.txt
```

## Documentation

See the `docs/` directory for detailed information:
- `docs/architecture.md`: System design and components.
- `docs/protocol.md`: Communication protocol specification.

## Testing

### Unit Tests
The project includes unit tests for core utilities (SHA256).
```bash
./build/bin/unit_tests
```

### Integration Tests
A Python script is provided to verify the full flow (Tracker + Seeder + Downloader).
```bash
python run_tests.py
```
*Note: This script requires Python 3 and builds the project automatically if needed.*
test change
