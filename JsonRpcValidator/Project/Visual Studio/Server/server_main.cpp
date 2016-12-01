#include <iostream>
#include <string>
#include <algorithm>

#include "server_socket.h"
#include "lexer.h"
#include "parser.h"
#include "json_rpc_validator.h"

#include "visualize_tree_visitor.h"
#include "validation_error_exception.h"



void PerformReceivingMessage();
void ParseSampleJson();
void ValidateSampleJsonRpc();
const std::string GetJsonPrcObjectTypeString(const JsonRpcObjectType type);

int main()
{
    //PerformReceivingMessage();
    //ParseSampleJson();
    ValidateSampleJsonRpc();

    system("pause");
    return 0;
}


void PerformReceivingMessage()
{
    // Initiate use of the Winsock DLL by a process.
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 1), &wsaData) != 0)
    {
        std::cout << "Initiating Winsock DLL error occurred." << std::endl;

        system("pause");
        return;
    }



    const int portNumber = 5000;
    ServerSocket socket(portNumber);

    if (!socket.IsSocketValid())
    {
        std::cout << "Socket is not valid, "
            << "you cannot do any communication." << std::endl;

        WSACleanup();

        system("pause");
        return;
    }

    try
    {
        socket.Bind();
        socket.Listen(1);

        ServerSocket newSocket = socket.Accept();

        std::string message = socket.Receive(newSocket);

        std::cout << "Received message:" << std::endl
            << message << std::endl;

    }
    catch (const ServerSocketException& exception)
    {
        std::cout << exception.what() << std::endl;
        WSACleanup();
    }
}
void ParseSampleJson()
{
    std::list<Token*> tokens;
    try
    {
        std::string text = 
            "[{ \"name\" : \"Ivan\", \"numbers\" : [10, -21, -1.5, +1.8e-7], \"nested object\" : { \"val\": true } },{}]";
    

        Lexer l;
        tokens = l.Tokenize(text);

        Parser p;
        NonTerminalNode* parseTree = p.Parse(tokens);

        VisualizeTreeVisitor visualizer(std::cout);
        parseTree->AcceptVisitor(visualizer);
    }
    catch (const ValidationErrorException& vee)
    {
        std::cout << vee.what() << " (line " << vee.GetLineNumber() << ")" << std::endl;
    }

    std::for_each(tokens.begin(), tokens.end(), [](const Token* t) { delete t; });
}
void ValidateSampleJsonRpc()
{
    std::list<Token*> tokens;
    NonTerminalNode* parseTree = nullptr;

    try
    {
        std::string jsonRpc =
            "{ \"jsonrpc\": \"2.0\", \"method\" : \"subtract\", \"params\" : {\"n\": 42}, \"id\" : 4 }";

        std::cout << jsonRpc << std::endl
                  << std::endl;

        Lexer l;
        tokens = l.Tokenize(jsonRpc);

        Parser p;
        parseTree = p.Parse(tokens);

        VisualizeTreeVisitor visualizer(std::cout);
        parseTree->AcceptVisitor(visualizer);
        std::cout << std::endl
                  << std::endl;

        JsonRpcValidator validator;
        JsonRpcObjectType objectType = validator.Validate(parseTree);

        std::string typeString = GetJsonPrcObjectTypeString(objectType);

        std::cout << "Specified object is a valid " << typeString << "." << std::endl
                  << std::endl;

    }
    catch (const ValidationErrorException& vee)
    {
        std::cout << vee.what() << " (line " << vee.GetLineNumber() << ")" << std::endl;
    }
    
    delete parseTree;
    std::for_each(tokens.begin(), tokens.end(), [](Token* token) { delete token; });
}

const std::string GetJsonPrcObjectTypeString(const JsonRpcObjectType type)
{
    switch (type)
    {
    case JsonRpcObjectType::REQUEST:        return "Request";
    case JsonRpcObjectType::RESPONSE:       return "Response";
    case JsonRpcObjectType::BATCH_REQUEST:  return "Batch Request";
    case JsonRpcObjectType::BATCH_RESPONSE: return "Batch Response";

    default: return "<unexpected>";
    }
}
