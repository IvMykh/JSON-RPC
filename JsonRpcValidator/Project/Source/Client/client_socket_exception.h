#ifndef CLIENT_SOCKET_EXCEPTION_H_
#define CLIENT_SOCKET_EXCEPTION_H_



#include "descriptive_exception.h"



class ClientSocketException
    : public DescriptiveException

{
public:
    ClientSocketException(const char* message);
};

#endif // CLIENT_SOCKET_EXCEPTION_H_
