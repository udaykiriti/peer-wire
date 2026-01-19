#!/bin/bash

# Navigate to project root
cd "$(dirname "$0")/.."

# Ensure build exists
if [ ! -f "build/bin/tracker.exe" ] && [ ! -f "build/bin/tracker" ]; then
    echo "Tracker executable not found. Please run 'scripts/build.sh' first."
    exit 1
fi

echo "Starting Tracker on port 8080..."
if [ -f "build/bin/tracker.exe" ]; then
    ./build/bin/tracker.exe
else
    ./build/bin/tracker
fi
