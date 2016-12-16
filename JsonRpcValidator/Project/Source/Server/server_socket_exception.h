#ifndef SERVER_SOCKET_EXCEPTION_H_
#define SERVER_SOCKET_EXCEPTION_H_



#include <exception>
#include <string>



class ServerSocketException
    : public std::exception
{
public:
    ServerSocketException(const char* message);

    virtual const char* what() const throw() override;

private:
    std::string     message_;
};

#endif // SERVER_SOCKET_EXCEPTION_H_
