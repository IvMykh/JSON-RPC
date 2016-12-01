#ifndef PARSE_TREE_NODE_
#define PARSE_TREE_NODE_

#include "node_type.h"
#include "parse_tree_visitor.h"



class ParseTreeNode
{
public:
    ParseTreeNode(const NodeType type);
    
    virtual             ~ParseTreeNode();

    const NodeType      GetType() const;
    virtual const int   GetLineNumber() const = 0;

    virtual bool        IsNodeTerminal() const;

    virtual void        AcceptVisitor(ParseTreeVisitor& visitor) = 0;

private:
    NodeType type_;
};

#endif // !PARSE_TREE_NODE_
