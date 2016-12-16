#include "non_terminal_node.h"

#include <iostream>



NonTerminalNode::NonTerminalNode(const NodeType type)
    : ParseTreeNode(type)
{
}

NonTerminalNode::~NonTerminalNode()
{
    for (auto child = children_.begin(); child != children_.end(); ++child)
    {
        if (!(*child)->IsNodeTerminal())
        {
            delete *child;
        }
    }

    children_.clear();
}

const std::list<ParseTreeNode*>& NonTerminalNode::GetChildren() const
{
    return children_;
}

const int NonTerminalNode::GetLineNumber() const
{
    return children_.front()->GetLineNumber();
}

void NonTerminalNode::PushFrontChild(ParseTreeNode* child)
{
    children_.push_front(child);
}

void NonTerminalNode::PushBackChild(ParseTreeNode* child)
{
    children_.push_back(child);
}

void NonTerminalNode::AcceptVisitor(ParseTreeVisitor& visitor)
{
    visitor.VisitNonTerminalNode(this);
}
