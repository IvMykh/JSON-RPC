#include <iostream>

#include "client_socket.h"

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
    const char* serverName = "127.0.0.1";

    ClientSocket socket(portNumber, serverName);

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
        socket.Connect();
        
        std::cout << "Please, enter a message to send:" << std::endl;
        std::string message;
        std::cin >> message;

        int bytesSent = socket.Send(message.c_str(), message.length());
        socket.ShutDown(ClientSocket::ShutDownOptions::SHUTDOWN_SEND);

        if (bytesSent != message.length())
        {
            std::cout << "Only " << bytesSent
                      << " symbols of your text were sent instead of "
                      << message.length() << std::endl;
        }
    }
    catch (const ClientSocketException& exception)
    {
        std::cout << exception.what() << std::endl;
        WSACleanup();
    }


    system("pause");
    return 0;
}
