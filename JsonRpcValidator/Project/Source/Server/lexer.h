#ifndef LEXER_H_
#define LEXER_H_

#include <list>
#include "token.h"



class Lexer
{
public:
    const std::list<Token*> 
            Tokenize(const std::string& text) const;

private:
    bool    isDelimiter(const char character) const;

    Token*  retrieveTrue  (const std::string& text, int lineNumber, int startPos) const;
    Token*  retrieveFalse (const std::string& text, int lineNumber, int startPos) const;
    Token*  retrieveNull  (const std::string& text, int lineNumber, int startPos) const;
    Token*  retrieveString(const std::string& text, int lineNumber, int startPos) const;
    Token*  retrieveNumber(const std::string& text, int lineNumber, int startPos) const;
};

#endif // !LEXER_H_
