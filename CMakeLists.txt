cmake_minimum_required(VERSION 3.10)
project(PeerWire)

set(CMAKE_CXX_STANDARD 17)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include common directories
include_directories(src/common)

# Source files for common utilities
set(COMMON_SOURCES
    src/common/sha256.cpp
    src/common/socket_utils.cpp
)

# Tracker Executable
add_executable(tracker
    src/tracker/tracker_main.cpp
    ${COMMON_SOURCES}
)

# Peer Daemon
add_executable(peer_daemon
    src/node/peer_daemon.cpp
    src/node/peer_node.cpp
    src/ipc/ipc_server.cpp
    ${COMMON_SOURCES}
)

# IPC Client (CLI Tool)
add_executable(send_cmd
    src/tools/ipc_client.cpp
    ${COMMON_SOURCES}
)

target_include_directories(peer_daemon PRIVATE src/node src/ipc)
target_include_directories(send_cmd PRIVATE src/common)

# Test Executable
add_executable(unit_tests
    src/tests/test_main.cpp
    ${COMMON_SOURCES}
)

if(WIN32)
    target_link_libraries(tracker ws2_32)
    target_link_libraries(peer_daemon ws2_32)
    target_link_libraries(send_cmd ws2_32)
    target_link_libraries(unit_tests ws2_32)
endif()
