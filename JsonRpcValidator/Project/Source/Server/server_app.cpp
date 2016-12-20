#include "server_app.h"

#include <iostream>
#include <fstream>
#include <ctime>
#include <algorithm>

#include "server_socket.h"
#include "lexer.h"
#include "parser.h"
#include "json_rpc_validator.h"
#include "validation_error_exception.h"



void RunApp()
{
#ifdef _WIN32
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 1), &wsaData) != 0)
    {
        throw ServerSocketException(
            "Initiating Winsock DLL error occurred.");
    }
#endif

    const int kPortNumber = 5000;
    ServerSocket socket(kPortNumber);

    if (!socket.IsSocketValid())
    {
#ifdef _WIN32
        WSACleanup();
#endif
        throw ServerSocketException(
            "Socket is not valid, you cannot perform any communication.");
    }

    try
    {
        socket.Bind();

        ShowStatus(
            "Started listening port # " + std::to_string(kPortNumber) + " ...",
            true);
        socket.Listen(2);
    }
    catch (const ServerSocketException& exception)
    {
        std::cout << exception.what() << std::endl;
        std::cout << "Server cannot be started" << std::endl
                  << std::endl;

        return;
    }

    while (true)
    {
        try
        {
            ServerSocket newSocket = socket.Accept();
            ShowStatus(
                "Client with IP " + newSocket.GetConnectedClientIp() +
                " successfully connected",
                true);

            std::string requestText = newSocket.ReceiveRequest();

            ShowStatus(
                "Received " + std::to_string(requestText.length()) +
                " characters of text.");

            unsigned validationResult = PerformValidation(
                requestText, newSocket.GetConnectedClientIp());

            newSocket.SendResponse(validationResult);
        }
        catch (const ServerSocketException& exception)
        {
            std::cout << exception.what() << std::endl;

#ifdef _WIN32
            WSACleanup();
#endif
        }

        std::cout
            << "----------------------------------------" << std::endl
            << std::endl;
    
    }
}

const unsigned PerformValidation(
    const std::string& jsonRpcText, const std::string ip)
{
    ShowStatus("Starting validation ...");
   
    std::list<Token*> tokens;
    NonTerminalNode* parseTree = nullptr;
    
    unsigned validationResult = 0;
    std::string resultMessage;

    try
    {
        Lexer lexer;
        auto tokens = lexer.Tokenize(jsonRpcText);

        Parser parser;
        auto parseTree = parser.Parse(tokens);

        JsonRpcValidator validator;
        JsonRpcObjectType jsonRpcType = validator.Validate(parseTree);

        resultMessage = "Valid " + GetJsonPrcObjectTypeString(jsonRpcType);
    }
    catch (const ValidationErrorException& vee)
    {
        resultMessage = vee.what();
        validationResult = vee.GetLineNumber();
    }

    ShowStatus("Validation finished.");

    delete parseTree;
    std::for_each(
        tokens.begin(), tokens.end(), [](Token* token) { delete token; });

    LogValidationResult(ip, resultMessage, validationResult);

    return validationResult;
}


void ShowStatus(const std::string message, const bool shouldSeparate)
{
    time_t rawtime;
    tm* timeinfo;

    const int bufferSize = 80;
    char buffer[bufferSize];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, bufferSize, "%d/%m/%Y %I:%M:%S", timeinfo);
    std::string dateTimeString(buffer);

    std::cout << "->  " << dateTimeString
              << "  " << message << std::endl;

    if (shouldSeparate)
    {
        std::cout << std::endl;
    }
}

void LogValidationResult(
    const std::string& ip,
    const std::string& message,
    const unsigned lineOfError)
{
    const std::string logFilePath = "Log.txt";

    std::ofstream logFileStream(logFilePath, std::ios::app);

    if (!logFileStream)
    {
        std::cout << "Cannot log file cannot be opened for writing." 
                  << std::endl;
        return;
    }

    time_t rawtime;
    tm* timeinfo;

    const int bufferSize = 80;
    char buffer[bufferSize];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, bufferSize, "%d/%m/%Y %I:%M:%S", timeinfo);
    std::string dateTimeString(buffer);

    logFileStream << "Time:       " << dateTimeString << std::endl
                  << "Client IP:  " << ip << std::endl
                  << "Result:     " << message << std::endl;

    if (lineOfError != 0)
    {
        logFileStream << "Line:       " << lineOfError << std::endl;
    }

    logFileStream 
        << std::endl 
        << std::endl;

    ShowStatus("Validation result successfully logged.", true);
}
