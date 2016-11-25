#ifndef TOKEN_H_
#define TOKEN_H_

#include <string>
#include "parse_tree_node.h"



class Token
    : public ParseTreeNode
{
public:
    Token();
    Token(const NodeType type, const std::string& value, const int lineNumber);

    virtual                 ~Token();

    const std::string&      GetValue() const;
    virtual const int       GetLineNumber() const override;

    virtual void            AcceptVisitor(ParseTreeVisitor& visitor) override;

    friend std::ostream&    operator<< (std::ostream& os, const Token& t);

private:
    int                     lineNumber_;
    std::string             value_;
};


#endif // !TOKEN_H_
