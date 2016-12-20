#include "validation_error_exception.h"



ValidationErrorException::ValidationErrorException(
    const int lineNumber, const char* msg)
    : DescriptiveException(msg), 
      lineNumber_(lineNumber)
{
}

const int ValidationErrorException::GetLineNumber() const
{
    return lineNumber_;
}
