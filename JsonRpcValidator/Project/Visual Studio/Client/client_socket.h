#ifndef CLIENT_SOCKET_H_
#define CLIENT_SOCKET_H_

#include <windows.h>
#include <winsock.h>
#include <string>

#include "client_socket_exception.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")



class ClientSocket
{
public:
    ClientSocket(const int serverPortNumber, const std::string& serverName);
    ~ClientSocket();

    const int           ServerPortNumber() const;
    const std::string&  ServerName() const;
    const bool          IsSocketValid() const;

    void                Connect() const;
    const int           Send(const char* clientRequest, const int messageLength) const;

private:
    static bool         performWsaStartup();
    
    static WSADATA      wsaData_;
    static bool         isWsaStartup_;
    static int          instancesCount_;

private:
    SOCKET              socketHandle_;
    sockaddr_in         serverAddress_;
    int                 serverPortNumber_;
    std::string         serverName_;
    bool                isSocketValid_;
};


#endif // CLIENT_SOCKET_H_