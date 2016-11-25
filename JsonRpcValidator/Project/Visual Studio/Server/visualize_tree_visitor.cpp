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
    outStream_ << intendation << "*--" << std::endl;

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
    outStream_ << intendation << token->GetValue() << std::endl;
}