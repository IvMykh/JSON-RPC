#ifndef CLIENT_SOCKET_H_
#define CLIENT_SOCKET_H_



#ifdef __linux__
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <netinet/in.h>
    #include <netdb.h>
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <errno.h>
    #include <arpa/inet.h>

#elif _WIN32
    #include <winsock.h>

    #pragma comment (lib, "Ws2_32.lib")
    #pragma comment (lib, "Mswsock.lib")
    #pragma comment (lib, "AdvApi32.lib")

#endif


#ifdef __linux__
    #define SOCKET int
    #define SOCKET_ERROR -1
    #define INVALID_SOCKET -1
#endif


#include <string>
#include "client_socket_exception.h"



class ClientSocket
{
public:
    enum ShutDownOptions
    {
        SHUTDOWN_RECEIVE    = 0,
        SHUTDOWN_SEND       = 1,
        SHUTDOWN_BOTH       = 2
    };


    ClientSocket(const int serverPortNumber, const std::string& serverName);
    ~ClientSocket();

    const int           GetServerPortNumber() const;
    const std::string&  GetServerName() const;
    const bool          IsSocketValid() const;

    void                Connect() const;

    const int           SendRequest(const char* data, const int length) const;
    const unsigned      ReceiveResponse() const;

    void                ShutDown(const int shutDownOption) const;

private:
    SOCKET              socketHandle_;
    sockaddr_in         serverAddress_;
    int                 serverPortNumber_;
    std::string         serverName_;
    bool                isSocketValid_;
};


#endif // !CLIENT_SOCKET_H_
