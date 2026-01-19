# Communication Protocol

PeerWire uses a custom binary protocol over TCP. All integers are little-endian (standard on x86).

## Packet Structure
Every packet starts with a standard header.

| Field | Size | Type | Description |
|---|---|---|---|
| Length | 4 bytes | uint32 | Length of payload following header |
| Type | 1 byte | uint8 | Packet Type ID |

## Packet Types

### REGISTER (Type 1)
Sent by a peer to establish its listening port with the Tracker.
- **Payload**: `[Port: uint16]`

### ADVERTISE_FILE (Type 5)
Sent by a peer to the Tracker to announce it has a file.
- **Payload**:
    - `File Hash`: 32 bytes (Raw SHA-256)
    - `File Size`: 8 bytes (uint64)
    - `Name Length`: 4 bytes (uint32)
    - `File Name`: Variable bytes (ASCII)

### REQUEST_PEERS (Type 3)
Sent by a downloader to the Tracker to find peers.
- **Payload**:
    - `File Hash`: 32 bytes (Raw SHA-256)

### RESPONSE_PEERS (Type 20)
Tracker response to REQUEST_PEERS.
- **Payload**:
    - `File Size`: 8 bytes (uint64)
    - `Peer Count`: 4 bytes (uint32)
    - **Repeated Peer List**:
        - `IP Length`: 1 byte (uint8)
        - `IP Address`: Variable bytes (String)
        - `Port`: 2 bytes (uint16)

### REQUEST_CHUNK (Type 10)
Sent by a downloader to a seeder to request a specific data block.
- **Payload**:
    - `File Hash`: 32 bytes
    - `Chunk Index`: 4 bytes (uint32)

### SEND_CHUNK (Type 11)
Response containing the data.
- **Payload**:
    - `File Hash`: 32 bytes
    - `Chunk Index`: 4 bytes (uint32)
    - `Data Size`: 4 bytes (uint32)
    - `Data`: Variable bytes (Raw content)
