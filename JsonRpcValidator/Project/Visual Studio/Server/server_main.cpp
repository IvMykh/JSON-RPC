#include <iostream>
#include <string>

#include "server_socket.h"

int main()
{
    // Initiate use of the Winsock DLL by a process.
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 1), &wsaData) != 0)
    {
        std::cout << "Initiating Winsock DLL error occurred." << std::endl;

        system("pause");
        return 0;
    }



    const int portNumber = 5000;
    ServerSocket socket(portNumber);

    if (!socket.IsSocketValid())
    {
        std::cout << "Socket is not valid, " 
                  << "you cannot do any communication." << std::endl;

        WSACleanup();

        system("pause");
        return 0;
    }

    try
    {
        socket.Bind();
        socket.Listen(1);

        ServerSocket newSocket = socket.Accept();

        std::string message = socket.Receive(newSocket);

        std::cout << "Received message:" << std::endl 
                  << message << std::endl;

    }
    catch (const ServerSocketException& exception)
    {
        std::cout << exception.what() << std::endl;
        WSACleanup();
    }

    system("pause");
    return 0;
}
