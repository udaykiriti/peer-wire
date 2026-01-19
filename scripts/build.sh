#!/bin/bash

# Navigate to project root
cd "$(dirname "$0")/.."

echo "Cleaning build directory..."
rm -rf build

echo "Configuring CMake..."
cmake -S . -B build

echo "Building project..."
cmake --build build --config Release

echo "Done! Executables are in build/bin/"
