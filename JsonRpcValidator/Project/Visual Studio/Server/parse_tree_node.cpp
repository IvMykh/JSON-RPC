#include "parse_tree_node.h"



ParseTreeNode::ParseTreeNode(const NodeType type)
    : type_(type)
{
}

ParseTreeNode::~ParseTreeNode()
{
}

const NodeType ParseTreeNode::GetType() const
{
    return type_;
}

bool ParseTreeNode::IsNodeTerminal() const
{
    return false;
}