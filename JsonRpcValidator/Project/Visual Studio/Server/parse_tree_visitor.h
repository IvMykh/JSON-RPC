#ifndef PARSE_TREE_VISITOR_
#define PARSE_TREE_VISITOR_



class NonTerminalNode;
class Token;



class ParseTreeVisitor
{
public:
    virtual void    VisitNonTerminalNode(NonTerminalNode* node) = 0;
    virtual void    VisitToken(Token* token) = 0;
};

#endif // !PARSE_TREE_VISITOR_