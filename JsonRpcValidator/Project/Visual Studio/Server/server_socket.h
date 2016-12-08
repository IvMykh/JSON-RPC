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
    enum ShutDownOptions
    {
        SHUTDOWN_RECEIVE    = 0,
        SHUTDOWN_SEND       = 1,
        SHUTDOWN_BOTH       = 2
    };


    ServerSocket(const int serverPortNumber);
    
    ~ServerSocket();

    const int           GetServerPortNumber() const;
    const bool          IsSocketValid() const;
    const std::string   GetConnectedClientIp() const;

    void                Bind() const;
    void                Listen(const int maxConnectionsCount) const;
    const ServerSocket  Accept() const;
    void                ShutDown(const int shutDownOption) const;

    const int           SendResponse(const unsigned& response) const;
    const std::string   ReceiveRequest() const;


private:
    ServerSocket(
        SOCKET socketHandle, 
        const int serverPortNumber, 
        const sockaddr_in* clientAddress);

    void                constructSocket(
        SOCKET socketHandle, const sockaddr_in* clientAddress);


    SOCKET              socketHandle_;
    sockaddr_in         serverAddress_;
    sockaddr_in         connectedClientAddress_;
    int                 serverPortNumber_;
    bool                isSocketValid_;
};

#endif // !SERVER_SOCKET_H_
