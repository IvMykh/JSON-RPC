#include <iostream>
#include <string>
#include <algorithm>

#include "server_socket.h"
#include "lexer.h"
#include "parser.h"
#include "visualize_tree_visitor.h"
#include "parsing_error_exception.h"

void PerfoemReceivingMessage();
void ParseSampleJson();

int main()
{
    //PerfoemReceivingMessage();
    ParseSampleJson();

    system("pause");
    return 0;
}


void PerfoemReceivingMessage()
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
        std::string text = "{ \"name\" : \"Ivan\", \"numbers\" : [10, -21, -1.5, +1.8e-7], \"nested object\" : { \"val\": true } }";
    

        Lexer l;
        tokens = l.Tokenize(text);

        Parser p;
        NonTerminalNode* parseTree = p.Parse(tokens);

        VisualizeTreeVisitor visualizer(std::cout);
        parseTree->AcceptVisitor(visualizer);
    }
    catch (ParsingErrorException pee)
    {
        std::cout << pee.what() << " (line " << pee.GetLineNumber() << ")" << std::endl;
    }

    std::for_each(tokens.begin(), tokens.end(), [](const Token* t) { delete t; });
}