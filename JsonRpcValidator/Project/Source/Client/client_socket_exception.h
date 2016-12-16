#ifndef CLIENT_SOCKET_EXCEPTION_H_
#define CLIENT_SOCKET_EXCEPTION_H_



#include <exception>
#include <string>



class ClientSocketException
    : public std::exception
{
public:
    ClientSocketException(const char* message);

    virtual const char* what() const throw() override;

private:
    std::string     message_;
};

#endif // CLIENT_SOCKET_EXCEPTION_H_
