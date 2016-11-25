#include "non_terminal_node.h"



NonTerminalNode::NonTerminalNode(const NodeType type)
    : ParseTreeNode(type)
{
}

NonTerminalNode::~NonTerminalNode()
{
}

const std::list<ParseTreeNode*>& NonTerminalNode::GetChildren() const
{
    return children_;
}

const int NonTerminalNode::GetLineNumber() const
{
    return children_.front()->GetLineNumber();
}

void NonTerminalNode::AddChild(ParseTreeNode* child)
{
    children_.push_back(child);
}

void NonTerminalNode::AcceptVisitor(ParseTreeVisitor& visitor)
{
    visitor.VisitNonTerminalNode(this);
}
