#include <iostream>

#include "client_socket.h"

int main()
{
    const int portNumber = 5000;
    const char* serverName = "127.0.0.1";

    ClientSocket socket(portNumber, serverName);

    if (!socket.IsSocketValid())
    {
        std::cout << "Socket is not valid, " 
                  << "you cannot do any communication." << std::endl;

        system("pause");
        return 0;
    }

    try
    {
        socket.Connect();

        std::cout << "Please, enter the text to send: ";
        std::string text;
        std::cin >> text;

        int bytesSent = socket.Send(text.c_str(), text.length());

        if (bytesSent != text.length())
        {
            std::cout << "Only " << bytesSent
                << " symbols of your text were sent instead of "
                << text.length() << std::endl;
        }
    }
    catch (const ClientSocketException& exception)
    {
        std::cout << exception.what() << std::endl;
    }

    system("pause");
    return 0;
}