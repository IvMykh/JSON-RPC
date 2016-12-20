#ifndef VALIDATION_ERROR_EXCEPTION_H_
#define VALIDATION_ERROR_EXCEPTION_H_



#include "descriptive_exception.h"



class ValidationErrorException 
    : public DescriptiveException
{
public:
    ValidationErrorException(const int lineNumber, const char* msg = "");

    const int               GetLineNumber() const;

private:
    int                     lineNumber_;
};

#endif // !VALIDATION_ERROR_EXCEPTION_H_
