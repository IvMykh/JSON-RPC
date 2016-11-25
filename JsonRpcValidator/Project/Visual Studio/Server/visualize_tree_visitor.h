#ifndef VISUALIZE_TREE_VISITOR_
#define VISUALIZE_TREE_VISITOR_

#include <iostream>

#include "parse_tree_visitor.h"
#include "non_terminal_node.h"
#include "token.h"



class VisualizeTreeVisitor
    : public ParseTreeVisitor
{
public:
    VisualizeTreeVisitor(std::ostream& outStream);

    virtual void    VisitNonTerminalNode(NonTerminalNode* node) override;
    virtual void    VisitToken(Token* token) override;

private:
    std::ostream&   outStream_;
    int             level_;
};

#endif // !VISUALIZE_TREE_VISITOR_