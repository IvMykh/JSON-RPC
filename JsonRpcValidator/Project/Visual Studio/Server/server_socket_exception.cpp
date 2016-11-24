#include "server_socket_exception.h"

ServerSocketException::ServerSocketException(const char* message)
    : std::exception(message)
{
}
