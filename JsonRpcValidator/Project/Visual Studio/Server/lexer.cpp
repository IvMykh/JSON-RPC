#include "lexer.h"

#include <regex>
#include "parsing_error_exception.h"


const std::list<Token*> Lexer::Tokenize(const std::string& text) const
{
    std::list<Token*> tokens;
    int lineNumber = 1;

    int linePos = 0;
    while (linePos < text.length())
    {
        const char firstChar = text[linePos];

        switch (firstChar)
        {
        case '{':  tokens.push_back(new Token { NodeType::OpenCurly, "{", lineNumber });
            break;
        case '}':  tokens.push_back(new Token { NodeType::CloseCurly, "}", lineNumber });
            break;
        case '[':  tokens.push_back(new Token { NodeType::OpenSquare, "[", lineNumber }); 
            break;
        case ']':  tokens.push_back(new Token { NodeType::CloseSquare, "]", lineNumber });
            break;
        case ',':  tokens.push_back(new Token { NodeType::Comma, ",", lineNumber });
            break;
        case ':':  tokens.push_back(new Token { NodeType::Colon, ":", lineNumber });
            break;

        case 't':  tokens.push_back(retrieveTrue(text, lineNumber, linePos)); 
            break;
        case 'f':  tokens.push_back(retrieveFalse(text, lineNumber, linePos)); 
            break;
        case 'n':  tokens.push_back(retrieveNull(text, lineNumber, linePos)); 
            break;
        case '\"': tokens.push_back(retrieveString(text, lineNumber, linePos)); 
            break;

        case ' ':
        case '\t':
            ++linePos;
            continue;

        case '\n':
            ++lineNumber;
            ++linePos;
            continue;

        default:
            if (firstChar == '+' ||
                firstChar == '-' ||
                ('0' <= firstChar && firstChar <= '9'))
            {
                tokens.push_back(retrieveNumber(text, lineNumber, linePos));
            }
            else
            {
                throw ParsingErrorException(lineNumber, "Invalid token");
            }
            break;
        }

        linePos += tokens.back()->GetValue().length();
    }

    return tokens;
}

bool Lexer::isDelimiter(const char character) const
{
    return character == ' ' ||
           character == '{' ||
           character == '}' ||
           character == '[' ||
           character == ']' ||
           character == ',' ||
           character == ':' ||
           character == '+' ||
           character == '-' ||
           character == '\n' ||
           ('0' <= character && character <= '9');
}

Token* Lexer::retrieveTrue(const std::string& text, int lineNumber, int startPos) const
{   
    if (text[startPos++] == 't' &&
        text[startPos++] == 'r' &&
        text[startPos++] == 'u' &&
        text[startPos++] == 'e' &&
        isDelimiter(text[startPos]))
    {
        return new Token(NodeType::True, "true", lineNumber);
    }
    else
    {
        throw ParsingErrorException(lineNumber, "Error occurred while parsing true");
    }
}

Token* Lexer::retrieveFalse(const std::string& text, int lineNumber, int startPos) const
{ 
    if (text[startPos++] == 'f' &&
        text[startPos++] == 'a' &&
        text[startPos++] == 'l' &&
        text[startPos++] == 's' &&
        text[startPos++] == 'e' &&
        isDelimiter(text[startPos]))
    {
        return new Token(NodeType::False, "false", lineNumber);
    }
    else
    {
        throw ParsingErrorException(lineNumber, "Error occurred while parsing false");
    }
}

Token* Lexer::retrieveNull(const std::string& text, int lineNumber, int startPos) const
{ 
    if (text[startPos++] == 'n' &&
        text[startPos++] == 'u' &&
        text[startPos++] == 'l' &&
        text[startPos++] == 'l' &&
        isDelimiter(text[startPos]))
    {
        return new Token(NodeType::Null, "null", lineNumber);
    }
    else
    {
        throw ParsingErrorException(lineNumber, "Error occurred while parsing null");
    }
}

Token* Lexer::retrieveString(const std::string& text, int lineNumber, int startPos) const
{ 
    std::regex rgx("^\\\"(\\\\[\\\"\\\\\\/bfnrt]|\\\\u[0-9a-fA-F]{4}|[^\"\\\\])*\"");
    std::smatch match;

    if (regex_search(text.begin() + startPos, text.end(), match, rgx))
    {
        return new Token(NodeType::String, match[0], lineNumber);
    }
    else
    {
        throw ParsingErrorException(lineNumber, "Error occurred while parsing string");
    }
}

Token* Lexer::retrieveNumber(const std::string& text, int lineNumber, int startPos) const
{ 
    std::regex rgx("^[+-]?[0-9]+(\\.[0-9]+)?([eE][+-]?[0-9]+)?");
    std::smatch match;

    if (regex_search(text.begin() + startPos, text.end(), match, rgx))
    {
        return new Token(NodeType::Number, match[0], lineNumber);
    }
    else
    {
        throw ParsingErrorException(lineNumber, "Error occurred while parsing number");
    }
}
