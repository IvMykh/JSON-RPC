#include "parser.h"

#include <exception>
#include <string>

#include "validation_error_exception.h"



Parser::Parser()
{
}

Parser::~Parser()
{
}

NonTerminalNode* Parser::Parse(const std::list<Token*>& tokens) const
{
    std::list<ParseTreeNode*> tokensToParse(tokens.begin(), tokens.end());
    NonTerminalNode* tree = nullptr;
    
    if (tokensToParse.empty())
    {
        throw ValidationErrorException(1, "'{' or '[' expected");
    }


    if (tokensToParse.front()->GetType() == NodeType::OpenSquare)
    {
        tree = parseArray(tokensToParse);
    }
    else if (tokensToParse.front()->GetType() == NodeType::OpenCurly)
    {
        tree = parseObject(tokensToParse);
    }
    else
    {
        throw ValidationErrorException(1, "'{' or '[' expected");
    }

    if (!tokensToParse.empty())
    {
        throw ValidationErrorException(
            tokensToParse.front()->GetLineNumber(), "End of object expected");
    }

    return tree;
}



NonTerminalNode* Parser::parseObject(std::list<ParseTreeNode*>& nodes) const
{
    std::list<ParseTreeNode*> objectNodes = 
        retrieveComplexObjectFrom(nodes, NodeType::OpenCurly);
    
    ParseTreeNode* openCurlyNode = objectNodes.front();
    objectNodes.pop_front();

    ParseTreeNode* closeCurlyNode = objectNodes.back();
    objectNodes.pop_back();
    
    
    NonTerminalNode* root = new NonTerminalNode(NodeType::Object);

    try
    {
        if (!objectNodes.empty())
        {
            root->PushBackChild(parsePair(objectNodes));
        }

        ParseTreeNode* previousNode = nullptr;

        while (!objectNodes.empty())
        {
            if (objectNodes.front()->GetType() != NodeType::Comma)
            {
                throw ValidationErrorException(
                    objectNodes.front()->GetLineNumber(), "',' expected");
            }

            previousNode = objectNodes.front();
            root->PushBackChild(objectNodes.front());
            objectNodes.pop_front();

            if (objectNodes.empty())
            {
                throw ValidationErrorException(
                    previousNode->GetLineNumber(), "Next pair expected");
            }

            root->PushBackChild(parsePair(objectNodes));
        }

        root->PushFrontChild(openCurlyNode);
        root->PushBackChild(closeCurlyNode);
    }
    catch (const ValidationErrorException& vee)
    {
        delete root;
        throw;
    }

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

    try
    {
        if (!arrayNodes.empty())
        {
            root->PushBackChild(parseValue(arrayNodes));
        }

        ParseTreeNode* previousNode = nullptr;

        while (!arrayNodes.empty())
        {
            if (arrayNodes.front()->GetType() != NodeType::Comma)
            {
                throw ValidationErrorException(
                    arrayNodes.front()->GetLineNumber(), "',' expected");
            }

            previousNode = arrayNodes.front();
            root->PushBackChild(arrayNodes.front());
            arrayNodes.pop_front();

            if (arrayNodes.empty())
            {
                throw ValidationErrorException(
                    previousNode->GetLineNumber(), "Next value expected");
            }

            root->PushBackChild(parseValue(arrayNodes));
        }

        root->PushFrontChild(openSquareNode);
        root->PushBackChild(closeSquareNode);
    }
    catch (const ValidationErrorException& vee)
    {
        delete root;
        throw;
    }

    return root;
}

NonTerminalNode* Parser::parsePair(std::list<ParseTreeNode*>& nodes) const
{
    NonTerminalNode* root = new NonTerminalNode(NodeType::Pair);

    try
    {
        if (nodes.front()->GetType() != NodeType::String)
        {
            throw ValidationErrorException(
                nodes.front()->GetLineNumber(), "String expected");
        }

        ParseTreeNode* previousNode = nodes.front();
        root->PushBackChild(nodes.front());
        nodes.pop_front();



        if (nodes.empty())
        {
            throw ValidationErrorException(
                previousNode->GetLineNumber(), "':' expected");
        }
        else if (nodes.front()->GetType() != NodeType::Colon)
        {
            throw ValidationErrorException(
                nodes.front()->GetLineNumber(), "':' expected");
        }

        previousNode = nodes.front();
        root->PushBackChild(nodes.front());
        nodes.pop_front();



        if (nodes.empty())
        {
            throw ValidationErrorException(
                previousNode->GetLineNumber(), "Valid value expected");
        }

        root->PushBackChild(parseValue(nodes));
    }
    catch (const ValidationErrorException& vee)
    {
        delete root;
        throw;
    }

    return root;
}

NonTerminalNode* Parser::parseValue(std::list<ParseTreeNode*>& nodes) const
{
    NonTerminalNode* root = new NonTerminalNode(NodeType::Value);

    try
    {
        NodeType type = nodes.front()->GetType();

        switch (type)
        {
        case NodeType::OpenCurly:
            root->PushBackChild(parseObject(nodes));
            break;

        case NodeType::OpenSquare:
            root->PushBackChild(parseArray(nodes));
            break;

        case NodeType::True:
        case NodeType::False:
        case NodeType::Null:
        case NodeType::String:
        case NodeType::Number:
            root->PushBackChild(nodes.front());
            nodes.pop_front();
            break;

        default:
            throw ValidationErrorException(
                nodes.front()->GetLineNumber(), "Valid value expected");
        }
    }
    catch (const ValidationErrorException& vee)
    {
        delete root;
        throw;
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
        throw ValidationErrorException(-1, "Unexpected type of bracket.");
    }

    ParseTreeNode* openNode = nodes.front();

    if (openNode->GetType() != openBracketType)
    {
        throw ValidationErrorException(
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
        throw ValidationErrorException(
            nestedObject.back()->GetLineNumber(), closeBracketErrorMessage);
    }

    return nestedObject;
}
