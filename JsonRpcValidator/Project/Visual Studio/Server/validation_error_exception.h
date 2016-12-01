#ifndef VALIDATION_ERROR_EXCEPTION_H_
#define VALIDATION_ERROR_EXCEPTION_H_

#include <exception>



class ValidationErrorException :
    public std::exception
{
public:
    ValidationErrorException(const int lineNumber, const char* msg = "");
    
    const int   GetLineNumber() const;

private:
    int         lineNumber_;
};

#endif // !VALIDATION_ERROR_EXCEPTION_H_
