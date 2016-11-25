#include "parsing_error_exception.h"



ParsingErrorException::ParsingErrorException(const int lineNumber, const char* msg)
    : exception(msg), lineNumber_(lineNumber)
{
}

const int ParsingErrorException::GetLineNumber() const
{
    return lineNumber_;
}
