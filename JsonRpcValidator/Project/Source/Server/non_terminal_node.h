#ifndef NON_TERMINAL_NODE_
#define NON_TERMINAL_NODE_

#include <list>
#include "parse_tree_node.h"



class NonTerminalNode
    : public ParseTreeNode
{
public:
    NonTerminalNode(const NodeType type);

    virtual                             ~NonTerminalNode();

    const std::list<ParseTreeNode*>&    GetChildren() const;
    virtual const int                   GetLineNumber() const override;

    void                                PushFrontChild(ParseTreeNode* child);
    void                                PushBackChild(ParseTreeNode* child);

    virtual void                        AcceptVisitor(ParseTreeVisitor& visitor) override;

private:
    std::list<ParseTreeNode*>           children_;
};

#endif // !NON_TERMINAL_NODE_
