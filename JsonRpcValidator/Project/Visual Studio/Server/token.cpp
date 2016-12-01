#include "Token.h"

#include <iostream>
#include <iomanip>



Token::Token()
    :  ParseTreeNode(NodeType::None), value_(""), lineNumber_(0)
{
}

Token::Token(const NodeType type, const std::string& value, const int line)
    : ParseTreeNode(type), value_(value), lineNumber_(line)
{
}

Token::~Token()
{
}

const std::string& Token::GetValue() const
{
    return value_;
}
const int Token::GetLineNumber() const
{
    return lineNumber_;
}

bool Token::IsNodeTerminal() const
{
    return true;
}

void Token::AcceptVisitor(ParseTreeVisitor& visitor)
{
    visitor.VisitToken(this);
}

std::ostream& operator<< (std::ostream& os, const Token& t)
{
    std::string tokenType;
    
    switch (t.GetType())
    {
    case NodeType::OpenCurly:   tokenType = "OpenCurly";   break;
    case NodeType::CloseCurly:  tokenType = "CloseCurly";  break;
    case NodeType::OpenSquare:  tokenType = "OpenSquare";  break;
    case NodeType::CloseSquare: tokenType = "CloseSquare"; break;
    case NodeType::Colon:       tokenType = "Colon";       break;
    case NodeType::Comma:       tokenType = "Comma";       break;
    case NodeType::True:        tokenType = "True";        break;
    case NodeType::False:       tokenType = "False";       break;
    case NodeType::Null:        tokenType = "Null";        break;
    case NodeType::String:      tokenType = "String";      break;
    case NodeType::Number:      tokenType = "Number";      break;

    default: tokenType = ""; break;
    }

    return os << "Type: " << std::setw(15) << std::left << tokenType << ' '
              << "Val: "  << std::setw(15) << std::left <<  t.value_ << ' '
              << "Line: " << t.lineNumber_;
}
