#include "client_socket_exception.h"

ClientSocketException::ClientSocketException(const char* message)
    : message_(message)
{
}

const char* ClientSocketException::what() const throw()
{
    return message_.c_str();
}
