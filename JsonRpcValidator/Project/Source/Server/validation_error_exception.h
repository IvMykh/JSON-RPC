#ifndef VALIDATION_ERROR_EXCEPTION_H_
#define VALIDATION_ERROR_EXCEPTION_H_

#include <exception>
#include <string>



class ValidationErrorException :
    public std::exception
{
public:
    ValidationErrorException(const int lineNumber, const char* msg = "");

    const int               GetLineNumber() const;
    virtual const char*     what() const throw() override;

private:
    int                     lineNumber_;
    std::string             message_;
};

#endif // !VALIDATION_ERROR_EXCEPTION_H_
