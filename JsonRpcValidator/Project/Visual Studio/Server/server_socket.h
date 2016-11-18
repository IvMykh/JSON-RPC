#ifndef SERVER_SOCKET_H_
#define SERVER_SOCKET_H_

#include <windows.h>
#include <winsock.h>

#include "server_socket_exception.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

class ServerSocket
{
public:
    ServerSocket(const int serverPortNumber);
    ServerSocket(SOCKET socketHandle, const int serverPortNumber);
    
    ~ServerSocket();

    const int ServerPortNumber() const;
    const bool IsSocketValid() const;

    void Bind() const;
    void Listen(const int maxConnectionsCount) const;

    const ServerSocket Accept() const;
    const int Receive(ServerSocket socket, char* clientRequestBuffer, int bufferLength) const;

private:
    static bool performWsaStartup();

private:
    void constructSocket(SOCKET socketHandle);

private:
    static WSADATA wsaData_;
    static bool isWsaStartup_;
    static int  instancesCount_;

private:
    SOCKET socketHandle_;
    sockaddr_in serverAddress_;
    int serverPortNumber_;
    bool isSocketValid_;
};

#endif // SERVER_SOCKET_H_