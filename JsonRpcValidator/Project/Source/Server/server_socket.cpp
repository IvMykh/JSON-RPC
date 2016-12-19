#include "server_socket.h"



ServerSocket::ServerSocket(const int serverPortNumber)
    : serverPortNumber_(serverPortNumber),
      isSocketValid_(true)
{
    constructSocket(0, nullptr);
}

ServerSocket::ServerSocket(
    SOCKET socketHandle, const int serverPortNumber, const sockaddr_in* clientAddress)
    : serverPortNumber_(serverPortNumber),
      isSocketValid_(true)
{
    constructSocket(socketHandle, clientAddress);
}

ServerSocket::~ServerSocket()
{
#ifdef __linux__
    close(socketHandle_);
#elif _WIN32
    closesocket(socketHandle_);
#endif
}

const int ServerSocket::GetServerPortNumber() const
{
    return serverPortNumber_;
}

const bool ServerSocket::IsSocketValid() const
{
    return isSocketValid_;
}

const std::string ServerSocket::GetConnectedClientIp() const
{
    return std::string(inet_ntoa(connectedClientAddress_.sin_addr));
}

void ServerSocket::Bind() const
{
    int bindResult = bind(socketHandle_,
        (sockaddr*)&serverAddress_,
        sizeof(sockaddr_in));

    if (bindResult != 0)
    {
#ifdef __linux__
        int yes = 1;
#elif _WIN32
        char yes = '1';
#endif

        int setsockoptResult = setsockopt(
            socketHandle_,
            SOL_SOCKET,
            SO_REUSEADDR,
            &yes,
            sizeof yes);


        if (setsockoptResult == -1)
        {
            throw ServerSocketException("Socket binding error occurred.");
        }
    }
}

void ServerSocket::Listen(const int maxConnectionsCount) const
{
    int listenResult = listen(socketHandle_, maxConnectionsCount);

    if (listenResult != 0)
    {
        throw ServerSocketException("Socket listening error occurred.");
    }
}

const ServerSocket ServerSocket::Accept() const
{
    sockaddr_in clientAddress;

#ifdef __linux__
    unsigned int sockAddrSize = sizeof(sockaddr_in);
#elif _WIN32
    int sockAddrSize = sizeof(sockaddr_in);
#endif

    SOCKET newSHandle = accept(socketHandle_,
        (sockaddr*)&clientAddress,
        &sockAddrSize);

    return ServerSocket(
        newSHandle, serverPortNumber_, &clientAddress);
}

void ServerSocket::ShutDown(const int shutDownOption) const
{
    int shutdownResult = shutdown(socketHandle_, shutDownOption);

    if (shutdownResult == SOCKET_ERROR)
    {
        throw ServerSocketException("Socket shutdown error occurred.");
    }
}

const int ServerSocket::SendResponse(const unsigned& response) const
{
    int bytesSent = send(
        socketHandle_, (char*)&response, sizeof(response), MSG_DONTROUTE);

    if (bytesSent == SOCKET_ERROR)
    {
        throw ServerSocketException("Data sending error in socket occurred.");
    }

    return bytesSent;
}

const std::string ServerSocket::ReceiveRequest() const
{
    const size_t bufferSize = 100;
    char buffer[bufferSize];

    int bytesReceived;
    std::string message;

    do
    {
        memset(buffer, '\0', bufferSize);
        bytesReceived = recv(socketHandle_, buffer, bufferSize, 0);

        if (bytesReceived == SOCKET_ERROR)
        {
            throw ServerSocketException("Socket receiving data error occurred.");
        }
        else
        {
            message.append(buffer, buffer + bytesReceived);
        }

    } while (bytesReceived == bufferSize);

    return message;
}

void ServerSocket::constructSocket(
    SOCKET socketHandle, const sockaddr_in* clientAddress)
{
    memset(&connectedClientAddress_, 0, sizeof(sockaddr_in));
    memset(&serverAddress_, 0, sizeof(sockaddr_in));
    
    socketHandle_ = socketHandle;

    if (socketHandle_ == 0)
    {
        socketHandle_ = socket(AF_INET, SOCK_STREAM, 0);
    }

    if (socketHandle_ == INVALID_SOCKET)
    {
        isSocketValid_ = false;
        return;
    }

    if (clientAddress != nullptr)
    {
        connectedClientAddress_ = *clientAddress;
    }

    serverAddress_.sin_family = AF_INET;
    serverAddress_.sin_port = htons(serverPortNumber_);
    serverAddress_.sin_addr.s_addr = INADDR_ANY;
}