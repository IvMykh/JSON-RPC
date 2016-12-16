#include "client_socket.h"



ClientSocket::ClientSocket(const int serverPortNumber, const std::string& serverName)
    : socketHandle_(0),
      serverPortNumber_(serverPortNumber),
      serverName_(serverName),
      isSocketValid_(true)
{
    if ((socketHandle_ = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        isSocketValid_ = false;
        return;
    }

    int sockAddrSize = sizeof(sockaddr_in);

    memset(&serverAddress_, 0, sockAddrSize);
    serverAddress_.sin_family = AF_INET;
    serverAddress_.sin_port = htons(serverPortNumber_);
    serverAddress_.sin_addr.s_addr = inet_addr(serverName_.c_str());
    
    if (serverAddress_.sin_addr.s_addr == INADDR_NONE)
    {
        isSocketValid_ = false;
        return;
    }
}

ClientSocket::~ClientSocket()
{
#ifdef __linux__
    close(socketHandle_);
#elif _WIN32
    closesocket(socketHandle_);
#endif
}

const int ClientSocket::GetServerPortNumber() const
{
    return serverPortNumber_;
}
const std::string& ClientSocket::GetServerName() const
{
    return serverName_;
}
const bool ClientSocket::IsSocketValid() const
{
    return isSocketValid_;
}

void ClientSocket::Connect() const
{
    int connectResult = connect(socketHandle_,
        (sockaddr*)&serverAddress_,
        sizeof(sockaddr_in));

    if (connectResult != 0)
    {
        throw ClientSocketException("Socket connection error occurred.");
    }
}

const int ClientSocket::SendRequest(const char* data, const int length) const
{
    int bytesSent = send(socketHandle_, data, length, MSG_DONTROUTE);

    if (bytesSent == SOCKET_ERROR)
    {
        throw ClientSocketException("Data sending error in socket occurred.");
    }

    return bytesSent;
}

const unsigned ClientSocket::ReceiveResponse() const
{
    unsigned response = 0;
    int bytesReceived = recv(
        socketHandle_, (char*)&response, sizeof(response), 0);

    if (bytesReceived == SOCKET_ERROR)
    {
        throw ClientSocketException("Socket receiving data error occurred.");
    }

    return response;
}

void ClientSocket::ShutDown(const int shutDownOption) const
{
    int shutdownResult = shutdown(socketHandle_, shutDownOption);
    
    if (shutdownResult == SOCKET_ERROR) 
    {
        throw ClientSocketException("Socket shutdown error occurred.");
    }
}
