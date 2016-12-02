#ifndef SERVER_SOCKET_H_
#define SERVER_SOCKET_H_


#include <winsock.h>
#include <string>

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

    const int           GetServerPortNumber() const;
    const bool          IsSocketValid() const;

    void                Bind() const;
    void                Listen(const int maxConnectionsCount) const;

    const ServerSocket  Accept() const;
    const std::string   Receive(ServerSocket socket) const;

private:
    void                constructSocket(SOCKET socketHandle);



    SOCKET              socketHandle_;
    sockaddr_in         serverAddress_;
    int                 serverPortNumber_;
    bool                isSocketValid_;
};

#endif // SERVER_SOCKET_H_
