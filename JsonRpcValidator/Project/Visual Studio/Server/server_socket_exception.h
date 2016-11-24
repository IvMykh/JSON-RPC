#ifndef SERVER_SOCKET_EXCEPTION_H_
#define SERVER_SOCKET_EXCEPTION_H_

#include <exception>

class ServerSocketException 
    : public std::exception
{
public:
    ServerSocketException(const char* message);
};

#endif // SERVER_SOCKET_EXCEPTION_H_
