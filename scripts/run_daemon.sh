#!/bin/bash

# Navigate to project root
cd "$(dirname "$0")/.."

# Ensure build exists
if [ ! -f "build/bin/peer_daemon.exe" ] && [ ! -f "build/bin/peer_daemon" ]; then
    echo "Daemon executable not found. Please run 'scripts/build.sh' first."
    exit 1
fi

P2P_PORT=$1
if [ -z "$P2P_PORT" ]; then
    P2P_PORT=9000
fi

CONTROL_PORT=$2
if [ -z "$CONTROL_PORT" ]; then
    CONTROL_PORT=9999
fi

echo "Starting Peer Daemon on P2P Port $P2P_PORT (Control Port $CONTROL_PORT)..."
if [ -f "build/bin/peer_daemon.exe" ]; then
    ./build/bin/peer_daemon.exe $P2P_PORT $CONTROL_PORT
else
    ./build/bin/peer_daemon $P2P_PORT $CONTROL_PORT
fi
