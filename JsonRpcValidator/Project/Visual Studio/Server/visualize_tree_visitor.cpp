#include "visualize_tree_visitor.h"



VisualizeTreeVisitor::VisualizeTreeVisitor(std::ostream& outStream)
    : outStream_(outStream), 
      level_(0)
{
}

void VisualizeTreeVisitor::VisitNonTerminalNode(NonTerminalNode* node) 
{
    auto currentChild = node->GetChildren().begin();

    std::string intendation(level_ * 4, ' ');
    outStream_ << intendation 
               << "[" << nodeTypeToString(node->GetType()) << "] " 
               << "*--" << std::endl;

    ++level_;

    while (currentChild != node->GetChildren().end())
    {
        (*currentChild)->AcceptVisitor(*this);
        ++currentChild;
    }
    
    --level_;
}

void VisualizeTreeVisitor::VisitToken(Token* token)
{
    std::string intendation(level_ * 4, ' ');
    outStream_ << intendation 
               << "[" << nodeTypeToString(token->GetType()) << "] " 
               << token->GetValue() << std::endl;
}

const std::string VisualizeTreeVisitor::nodeTypeToString(const NodeType& nodeType) const
{
    switch (nodeType)
    {
    case NodeType::OpenCurly:   return "OpenCurly";
    case NodeType::CloseCurly:  return "CloseCurly";
    case NodeType::OpenSquare:  return "OpenSquare";
    case NodeType::CloseSquare: return "CloseSquare";
    case NodeType::Colon:       return "Colon";
    case NodeType::Comma:       return "Comma";
    case NodeType::True:        return "True";
    case NodeType::False:       return "False";
    case NodeType::Null:        return "Null";
    case NodeType::String:      return "String";
    case NodeType::Number:      return "Number";

    case NodeType::Object:      return "Object";
    case NodeType::Pair:        return "Pair";
    case NodeType::Array:       return "Array";
    case NodeType::Value:       return "Value";

    default: return "";
    }
}