#include "client_socket_exception.h"

ClientSocketException::ClientSocketException(const char* message)
    : std::exception(message)
{
}