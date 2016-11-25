#ifndef PARSING_ERROR_EXCEPTION_H_
#define PARSING_ERROR_EXCEPTION_H_

#include <exception>



class ParsingErrorException :
    public std::exception
{
public:
    ParsingErrorException(const int lineNumber, const char* msg = "");
    
    const int   GetLineNumber() const;

private:
    int         lineNumber_;
};

#endif // !PARSING_ERROR_EXCEPTION_H_
