#!/bin/bash

# Navigate to project root
cd "$(dirname "$0")/.."

CONTROL_PORT=9999
CMD_TOOL="./build/bin/send_cmd.exe"

# Colors
GREEN='\033[0;32m'
CYAN='\033[0;36m'
RED='\033[0;31m'
NC='\033[0m'

if [ ! -f "$CMD_TOOL" ]; then
    CMD_TOOL="./build/bin/send_cmd"
fi

if [ ! -f "$CMD_TOOL" ]; then
    echo "Command tool not found. Run scripts/build.sh"
    exit 1
fi

echo -e "${CYAN}========================================${NC}"
echo -e "${CYAN}      PeerWire TUI (Client Mode)       ${NC}"
echo -e "${CYAN}========================================${NC}"

while true; do
    echo -ne "${GREEN}PeerWire> ${NC}"
    read -r line
    
    if [[ "$line" == "exit" ]]; then
        break
    elif [[ "$line" == "help" ]]; then
         echo "Available Commands:"
         echo "  seed <path> (e.g., ./test_file.txt)"
         echo "  download <hash> <out>"
         echo "  tracker <ip> <port>"
         echo "  ping"
         echo "  exit"
    elif [[ -n "$line" ]]; then
         $CMD_TOOL $CONTROL_PORT $line
    fi
done
