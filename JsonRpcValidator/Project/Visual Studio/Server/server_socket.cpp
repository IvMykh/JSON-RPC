#include "server_socket.h"



ServerSocket::ServerSocket(const int serverPortNumber)
    : serverPortNumber_(serverPortNumber),
      isSocketValid_(true)
{
    constructSocket(NULL);
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

    --instancesCount_;
    if (instancesCount_ == 0)
    {
        WSACleanup();
        isWsaStartup_ = false;
    }
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

const int ServerSocket::Receive(ServerSocket socket, char* clientRequestBuffer, int bufferLength) const
{
    int bytesReceived = recv(socket.socketHandle_,
        clientRequestBuffer,
        bufferLength,
        MSG_PEEK);

    if (bytesReceived == SOCKET_ERROR)
    {
        throw ServerSocketException("Socket receiving data error");
    }

    return bytesReceived;
}



bool ServerSocket::performWsaStartup()
{
    int wsaStartupResult = WSAStartup(MAKEWORD(2, 1), &wsaData_);
    return wsaStartupResult == 0;
}

void ServerSocket::constructSocket(SOCKET socketHandle)
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

    socketHandle_ = socketHandle;

    if (socketHandle_ == NULL)
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


WSADATA ServerSocket::wsaData_;
bool ServerSocket::isWsaStartup_ = false;
int ServerSocket::instancesCount_ = 0;
