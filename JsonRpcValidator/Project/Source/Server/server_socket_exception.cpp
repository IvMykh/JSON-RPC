#include "server_socket_exception.h"



ServerSocketException::ServerSocketException(const char* message)
    : DescriptiveException(message)
{
}
