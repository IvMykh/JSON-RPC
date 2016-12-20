#ifndef DESCRIPTIVE_EXCEPTION_H_
#define DESCRIPTIVE_EXCEPTION_H_



#include <exception>
#include <string>



class DescriptiveException
    : public std::exception
{
public:
    DescriptiveException(const char* message);

    virtual const char* what() const throw() override;

private:
    std::string     message_;
};

#endif // !DESCRIPTIVE_EXCEPTION_H_
