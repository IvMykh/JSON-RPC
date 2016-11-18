#include "client_socket.h"


ClientSocket::ClientSocket(const int serverPortNumber, const std::string& serverName)
    : serverPortNumber_(serverPortNumber),
      serverName_(serverName),
      isSocketValid_(true)
{
    ++instancesCount_;

    if (!isWsaStartup_)
    {
        isWsaStartup_ = performWsaStartup();

        if (!isWsaStartup_)
        {
            isSocketValid_ = false;
            return;
        }
    }

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
    closesocket(socketHandle_);

    --instancesCount_;
    if (instancesCount_ == 0)
    {
        WSACleanup();
        isWsaStartup_ = false;
    }
}

const int ClientSocket::ServerPortNumber() const
{
    return serverPortNumber_;
}
const std::string& ClientSocket::ServerName() const
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

const int ClientSocket::Send(const char* clientRequest, const int messageLength) const
{
    int bytesSent = send(socketHandle_, clientRequest, messageLength, MSG_DONTROUTE);

    if (bytesSent == SOCKET_ERROR)
    {
        throw ClientSocketException("Data sending error in socket occurred.");
    }

    return bytesSent;
}



bool ClientSocket::performWsaStartup()
{
    int wsaStartupResult = WSAStartup(MAKEWORD(2, 1), &wsaData_);
    return wsaStartupResult == 0;
}

int ClientSocket::instancesCount_ = 0;
WSADATA ClientSocket::wsaData_;
bool ClientSocket::isWsaStartup_ = false;
