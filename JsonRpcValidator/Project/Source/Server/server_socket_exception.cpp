#include "server_socket_exception.h"

ServerSocketException::ServerSocketException(const char* message)
    : message_(message)
{
}

const char* ServerSocketException::what() const throw()
{
    return message_.c_str();
}
