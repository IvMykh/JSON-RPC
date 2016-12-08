#include "server_app.h"

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <algorithm>

#include "server_socket.h"
#include "lexer.h"
#include "parser.h"
#include "json_rpc_validator.h"
#include "validation_error_exception.h"



void RunApp()
{
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 1), &wsaData) != 0)
    {
        throw ServerSocketException(
            "Initiating Winsock DLL error occurred.");
    }

    const int kPortNumber = 5000;
    ServerSocket socket(kPortNumber);

    if (!socket.IsSocketValid())
    {
        WSACleanup();

        throw ServerSocketException(
            "Socket is not valid, you cannot perform any communication.");
    }

    try
    {
        socket.Bind();

        ShowStatus(
            "Started listening port # " + std::to_string(kPortNumber) + " ...", 
            true);
        socket.Listen(1);

        ServerSocket newSocket = socket.Accept();
        std::string requestText = newSocket.ReceiveRequest();

        ShowStatus(
            "Received " + std::to_string(requestText.length()) + 
            " characters of text.");

        unsigned validationResult = PerformValidation(requestText);
        newSocket.SendResponse(validationResult);
    }
    catch (const ServerSocketException& exception)
    {
        std::cout << exception.what() << std::endl;
        WSACleanup();
    }
}

const unsigned PerformValidation(const std::string& jsonRpcText)
{
    ShowStatus("Starting validation ...");
   
    std::list<Token*> tokens;
    NonTerminalNode* parseTree = nullptr;
    
    unsigned validationResult = 0;

    try
    {
        Lexer lexer;
        auto tokens = lexer.Tokenize(jsonRpcText);

        Parser parser;
        auto parseTree = parser.Parse(tokens);

        JsonRpcValidator validator;
        JsonRpcObjectType jsonRpcType = validator.Validate(parseTree);

        std::string typeString = GetJsonPrcObjectTypeString(jsonRpcType);

        ShowStatus("Validation succeeded!", true);
    }
    catch (const ValidationErrorException& vee)
    {
        ShowStatus(
            "Validation error occurred! (line " + 
            std::to_string(vee.GetLineNumber()) + ")", 
            true);

        validationResult = vee.GetLineNumber();
    }

    delete parseTree;
    std::for_each(
        tokens.begin(), tokens.end(), [](Token* token) { delete token; });

    return validationResult;
}


void ShowStatus(const std::string message, const bool shouldSeparate)
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::cout << "->  " << std::put_time(std::localtime(&in_time_t), "%X")
        << "  " << message << std::endl;

    if (shouldSeparate)
    {
        std::cout << std::endl;
    }
}
