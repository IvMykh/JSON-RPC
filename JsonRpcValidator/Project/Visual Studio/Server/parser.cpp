#include "Parser.h"

#include <string>
#include "parsing_error_exception.h"



Parser::Parser()
{
}


Parser::~Parser()
{
}

NonTerminalNode* Parser::Parse(const std::list<Token*>& tokens) const
{
    std::list<ParseTreeNode*> tokensToParse(tokens.begin(), tokens.end());
    NonTerminalNode* tree = parseObject(tokensToParse);

    if (!tokensToParse.empty())
    {
        throw ParsingErrorException(
            tokensToParse.front()->GetLineNumber(), "End of object expected");
    }

    return tree;
}



NonTerminalNode* Parser::parseObject(std::list<ParseTreeNode*>& nodes) const
{
    // Only for main object.
    if (nodes.empty())
    {
        throw ParsingErrorException(1, "'{' expected");
    }

    std::list<ParseTreeNode*> objectNodes = 
        retrieveComplexObjectFrom(nodes, NodeType::OpenCurly);
    
    ParseTreeNode* openCurlyNode = objectNodes.front();
    objectNodes.pop_front();

    ParseTreeNode* closeCurlyNode = objectNodes.back();
    objectNodes.pop_back();
    
    
    NonTerminalNode* root = new NonTerminalNode(NodeType::Object);
    root->AddChild(openCurlyNode);

    if (!objectNodes.empty())
    {
        root->AddChild(parsePair(objectNodes));
    }

    ParseTreeNode* previousNode = nullptr;

    while (!objectNodes.empty())
    {
        if (objectNodes.front()->GetType() != NodeType::Comma)
        {
            throw ParsingErrorException(
                objectNodes.front()->GetLineNumber(), "',' expected");
        }

        previousNode = objectNodes.front();
        root->AddChild(objectNodes.front());
        objectNodes.pop_front();

        if (objectNodes.empty())
        {
            throw ParsingErrorException(
                previousNode->GetLineNumber(), "Next pair expected");
        }

        root->AddChild(parsePair(objectNodes));
    }

    root->AddChild(closeCurlyNode);

    return root;
}

NonTerminalNode* Parser::parseArray(std::list<ParseTreeNode*>& nodes) const
{    
    std::list<ParseTreeNode*> arrayNodes =
        retrieveComplexObjectFrom(nodes, NodeType::OpenSquare);

    ParseTreeNode* openSquareNode = arrayNodes.front();
    arrayNodes.pop_front();

    ParseTreeNode* closeSquareNode = arrayNodes.back();
    arrayNodes.pop_back();


    NonTerminalNode* root = new NonTerminalNode(NodeType::Array);
    root->AddChild(openSquareNode);

    if (!arrayNodes.empty())
    {
        root->AddChild(parseValue(arrayNodes));
    }

    ParseTreeNode* previousNode = nullptr;

    while (!arrayNodes.empty())
    {
        if (arrayNodes.front()->GetType() != NodeType::Comma)
        {
            throw ParsingErrorException(
                arrayNodes.front()->GetLineNumber(), "',' expected");
        }

        previousNode = arrayNodes.front();
        root->AddChild(arrayNodes.front());
        arrayNodes.pop_front();

        if (arrayNodes.empty())
        {
            throw ParsingErrorException(
                previousNode->GetLineNumber(), "Next value expected");
        }

        root->AddChild(parseValue(arrayNodes));
    }

    root->AddChild(closeSquareNode);

    return root;
}

NonTerminalNode* Parser::parsePair(std::list<ParseTreeNode*>& nodes) const
{
    NonTerminalNode* root = new NonTerminalNode(NodeType::Pair);

    if (nodes.front()->GetType() != NodeType::String)
    {
        throw ParsingErrorException(
            nodes.front()->GetLineNumber(), "String expected");
    }

    ParseTreeNode* previousNode = nodes.front();
    root->AddChild(nodes.front());
    nodes.pop_front();



    if (nodes.empty())
    {
        throw ParsingErrorException(
            previousNode->GetLineNumber(), "':' expected");
    } 
    else if(nodes.front()->GetType() != NodeType::Colon)
    {
        throw ParsingErrorException(
            nodes.front()->GetLineNumber(), "':' expected");
    }

    previousNode = nodes.front();
    root->AddChild(nodes.front());
    nodes.pop_front();



    if (nodes.empty())
    {
        throw ParsingErrorException(
            previousNode->GetLineNumber(), "Valid value expected");
    }

    root->AddChild(parseValue(nodes));
    
    return root;
}

NonTerminalNode* Parser::parseValue(std::list<ParseTreeNode*>& nodes) const
{
    NonTerminalNode* root = new NonTerminalNode(NodeType::Value);
    NodeType type = nodes.front()->GetType();

    switch (type)
    {
    case NodeType::OpenCurly:
        root->AddChild(parseObject(nodes));
        break;

    case NodeType::OpenSquare:
        root->AddChild(parseArray(nodes));
        break;

    case NodeType::True: 
    case NodeType::False:
    case NodeType::Null:
    case NodeType::String:
    case NodeType::Number:
        root->AddChild(nodes.front());
        nodes.pop_front();
        break;
  
    default:
        throw ParsingErrorException(
            nodes.front()->GetLineNumber(), "Valid value expected");
    }

    return root;
}


std::list<ParseTreeNode*> Parser::retrieveComplexObjectFrom(
    std::list<ParseTreeNode*>& nodes, const NodeType openBracketType) const
{
    NodeType closeBracketType;
    const char* openBracketErrorMessage;
    const char* closeBracketErrorMessage;

    if (openBracketType == NodeType::OpenCurly)
    {
        closeBracketType = NodeType::CloseCurly;
        openBracketErrorMessage = "{ expected";
        closeBracketErrorMessage = "} expected";
    }
    else if (openBracketType == NodeType::OpenSquare)
    {
        closeBracketType = NodeType::CloseSquare;
        openBracketErrorMessage = "[ expected";
        closeBracketErrorMessage = "] expected";
    }
    else
    {
        throw std::exception("Unexpected type of bracket."); 
    }

    ParseTreeNode* openNode = nodes.front();

    if (openNode->GetType() != openBracketType)
    {
        throw ParsingErrorException(
            openNode->GetLineNumber(), openBracketErrorMessage);
    }


    std::list<ParseTreeNode*> nestedObject;
    nestedObject.push_back(nodes.front());
    nodes.pop_front();

    int depth = 1;

    while (!(nodes.empty() || depth == 0))
    {
        if (nodes.front()->GetType() == openBracketType)
        {
            ++depth;
        }

        if (nodes.front()->GetType() == closeBracketType)
        {
            --depth;
        }

        nestedObject.push_back(nodes.front());
        nodes.pop_front();
    }

    if (nestedObject.back()->GetType() != closeBracketType)
    {
        throw ParsingErrorException(
            nestedObject.back()->GetLineNumber(), closeBracketErrorMessage);
    }

    return nestedObject;
}
