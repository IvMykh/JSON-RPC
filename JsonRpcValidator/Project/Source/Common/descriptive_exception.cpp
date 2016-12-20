#include "descriptive_exception.h"



DescriptiveException::DescriptiveException(const char* message)
    : message_(message)
{
}

const char* DescriptiveException::what() const throw()
{
    return message_.c_str();
}
