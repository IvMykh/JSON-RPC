#ifndef PARSER_H_
#define PARSER_H_

#include <list>

#include "token.h"
#include "non_terminal_node.h"



class Parser
{
public:
    Parser();
    ~Parser();

    NonTerminalNode* Parse(const std::list<Token*>& tokens) const;

private:
    NonTerminalNode* parseObject(std::list<ParseTreeNode*>& nodes) const;
    NonTerminalNode* parseArray(std::list<ParseTreeNode*>& nodes) const;
    NonTerminalNode* parsePair(std::list<ParseTreeNode*>& nodes) const;
    NonTerminalNode* parseValue(std::list<ParseTreeNode*>& nodes) const;

    std::list<ParseTreeNode*> retrieveComplexObjectFrom(
        std::list<ParseTreeNode*>& nodes, const NodeType openBracketType) const;
};

#endif // !PARSER_H_
