#ifndef SERVER_SOCKET_EXCEPTION_H_
#define SERVER_SOCKET_EXCEPTION_H_



#include "descriptive_exception.h"



class ServerSocketException
    : public DescriptiveException
{
public:
    ServerSocketException(const char* message);
};

#endif // SERVER_SOCKET_EXCEPTION_H_
