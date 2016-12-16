#include "validation_error_exception.h"



ValidationErrorException::ValidationErrorException(const int lineNumber, const char* msg)
    : lineNumber_(lineNumber),
      message_(msg)
{
}

const int ValidationErrorException::GetLineNumber() const
{
    return lineNumber_;
}

const char* ValidationErrorException::what() const throw()
{
    return message_.c_str();
}
