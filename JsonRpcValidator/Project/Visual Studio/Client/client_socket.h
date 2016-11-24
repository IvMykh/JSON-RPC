#ifndef CLIENT_SOCKET_H_
#define CLIENT_SOCKET_H_


#include <winsock.h>
#include <string>

#include "client_socket_exception.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


class ClientSocket
{
public:
    enum ShutDownOptions
    {
        SHUTDOWN_RECEIVE    = 0,
        SHUTDOWN_SEND       = 1,
        SHUTDOWN_BOTH       = 2
    };

public:
    ClientSocket(const int serverPortNumber, const std::string& serverName);
    ~ClientSocket();

    const int           ServerPortNumber() const;
    const std::string&  ServerName() const;
    const bool          IsSocketValid() const;

    void                Connect() const;
    void                ShutDown(const int shutDownOption) const;
    const int           Send(const char* clientRequest, const int messageLength) const;

private:
    SOCKET              socketHandle_;
    sockaddr_in         serverAddress_;
    int                 serverPortNumber_;
    std::string         serverName_;
    bool                isSocketValid_;
};


#endif // CLIENT_SOCKET_H_
