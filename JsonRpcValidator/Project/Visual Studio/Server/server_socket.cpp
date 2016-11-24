#include "server_socket.h"



ServerSocket::ServerSocket(const int serverPortNumber)
    : serverPortNumber_(serverPortNumber),
      isSocketValid_(true)
{
    constructSocket(0);
}
ServerSocket::ServerSocket(SOCKET socketHandle, const int serverPortNumber)
    : serverPortNumber_(serverPortNumber),
      isSocketValid_(true)
{
    constructSocket(socketHandle);
}

ServerSocket::~ServerSocket()
{
    closesocket(socketHandle_);
}

const int ServerSocket::ServerPortNumber() const
{
    return serverPortNumber_;
}
const bool ServerSocket::IsSocketValid() const
{
    return isSocketValid_;
}

void ServerSocket::Bind() const
{
    int bindResult = bind(socketHandle_,
        (sockaddr*)&serverAddress_,
        sizeof(sockaddr_in));

    if (bindResult != 0)
    {
        throw ServerSocketException("Socket binding error occurred.");
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
    sockaddr_in clientAddr;
    int sockAddrSize = sizeof(sockaddr_in);

    SOCKET newSHandle = accept(socketHandle_,
        (sockaddr*)&clientAddr,
        &sockAddrSize);

    return ServerSocket(newSHandle, serverPortNumber_);
}

const std::string ServerSocket::Receive(ServerSocket socket) const
{
    const size_t bufferSize = 10;
    char buffer[bufferSize];

    int bytesReceived;
    std::string message;
    
    do 
    {   
        memset(buffer, '\0', bufferSize);
        bytesReceived = recv(socket.socketHandle_, buffer, bufferSize, 0);

        if (bytesReceived == SOCKET_ERROR) 
        {
            throw ServerSocketException("Socket receiving data error occurred.");
        }
        else 
        {
            message.append(buffer, buffer + bufferSize);
        }

    } while (bytesReceived == bufferSize);
        
    return message;
}

void ServerSocket::constructSocket(SOCKET socketHandle)
{
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

    int sockAddrSize = sizeof(sockaddr_in);

    memset(&serverAddress_, 0, sockAddrSize);
    serverAddress_.sin_family = AF_INET;
    serverAddress_.sin_port = htons(serverPortNumber_);
    serverAddress_.sin_addr.s_addr = INADDR_ANY;
}
