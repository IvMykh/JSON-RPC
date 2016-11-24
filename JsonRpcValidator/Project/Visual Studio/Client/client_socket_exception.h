#ifndef CLIENT_SOCKET_EXCEPTION_H_
#define CLIENT_SOCKET_EXCEPTION_H_

#include <exception>

class ClientSocketException
    : public std::exception
{
public:
    ClientSocketException(const char* message);
};

#endif // CLIENT_SOCKET_EXCEPTION_H_
