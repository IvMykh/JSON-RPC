#include <iostream>
#include <string>

#include "server_socket.h"

int main()
{
    const int portNumber = 5000;
    ServerSocket socket(portNumber);

    if (!socket.IsSocketValid())
    {
        std::cout << "Socket is not valid, " 
                  << "you cannot do any communication." << std::endl;

        system("pause");
        return 0;
    }

    try
    {
        socket.Bind();
        socket.Listen(2);

        ServerSocket newSocket = socket.Accept();

        const int requestMsgSize = 200;
        char clientRequest[requestMsgSize] = "\0";

        int bytesReceived = socket.Receive(newSocket,
            clientRequest,
            requestMsgSize);

        std::string receivedText(clientRequest);

        std::cout << "Server received " << bytesReceived << " bytes" << std::endl;
        std::cout << "Text: " << receivedText << std::endl;

        memset(clientRequest, 0, requestMsgSize);
    }
    catch (const ServerSocketException& exception)
    {
        std::cout << exception.what() << std::endl;
    }

    system("pause");
    return 0;
}