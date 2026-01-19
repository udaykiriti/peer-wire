#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H

#include <string>
#include <vector>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    typedef SOCKET SocketType;
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    typedef int SocketType;
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
#endif

class SocketUtils {
public:
    static bool init();
    static void cleanup();
    static SocketType createSocket();
    static bool bindSocket(SocketType sock, int port);
    static bool listenSocket(SocketType sock);
    static SocketType acceptConnection(SocketType sock, std::string& clientIp);
    static bool connectToServer(SocketType sock, const std::string& ip, int port);
    static void closeSocket(SocketType sock);

    static bool sendAll(SocketType sock, const void* data, size_t size);
    static bool recvAll(SocketType sock, void* data, size_t size);
};

#endif // SOCKET_UTILS_H
