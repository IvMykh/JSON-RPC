#include "client_app.h"

#include <iostream>
#include <fstream>
#include <ctime>

#include "client_socket.h"
#include "descriptive_exception.h"



void RunApp()
{
    const std::string kDefaultFilePath = "JSON-RPC Sample.json";
    
    std::string sourceFilePath;
    std::cout << "Please, enter target JSON-RPC file path" << std::endl
              << "[enter '.' for default file (\"" << kDefaultFilePath << "\")]:"
              << std::endl;

    std::cout << std::endl
              << "Your input: ";
    std::getline(std::cin, sourceFilePath);

    if (sourceFilePath.length() == 1 && sourceFilePath[0] == '.')
    {
        sourceFilePath = kDefaultFilePath;
        std::cout << std::endl
                  << "File \"" << sourceFilePath << "\" selected." << std::endl;
    }

    std::cout << std::endl;

    try
    {
        ShowStatus("Reading file \"" + sourceFilePath + "\" ...");
        
        std::vector<char> bytesToSend;
        ReadFileBinary(sourceFilePath, bytesToSend);        
        ShowStatus(
            "File \"" + sourceFilePath + "\" has been read successfully.", 
            true);

        const unsigned response = PerformCommunication(bytesToSend);

        if (response == 0)
        {
            ShowStatus("RESULT: specified JSON-RPC file is valid!", true);
        }
        else
        {
            ShowStatus(
                "RESULT: specified JSON-RPC file is NOT valid. " +
                ("An error occurred in line " + std::to_string(response)), 
                true);
        }
    }
    catch (const std::exception& exception)
    {
        ShowStatus(exception.what(), true);

#ifdef _WIN32
        WSACleanup();
#endif
    }
}

const unsigned PerformCommunication(const std::vector<char>& dataToSend)
{
#ifdef _WIN32
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 1), &wsaData) != 0)
    {
        throw ClientSocketException(
            "Initiating Winsock DLL error occurred.");
    }
#endif

    const int kPortNumber = 5000;
    const char* kServerName = "127.0.0.1";

    ClientSocket socket(kPortNumber, kServerName);

    if (!socket.IsSocketValid())
    {
#ifdef _WIN32
        WSACleanup();
#endif

        throw ClientSocketException(
            "Socket is not valid, you cannot perform any communication.");
    }

    socket.Connect();

    ShowStatus("Sending " + std::to_string(dataToSend.size()) + " bytes ...");

    int bytesSent = socket.SendRequest(dataToSend.data(), dataToSend.size());
    socket.ShutDown(ClientSocket::ShutDownOptions::SHUTDOWN_SEND);

    if (bytesSent != dataToSend.size())
    {
        ShowStatus(
            "WARNING: only " + std::to_string(bytesSent) +
            " bytes were sent instead of " + std::to_string(dataToSend.size()) + ".",
            true);
    }
    else
    {
        ShowStatus("Sent successfully.", true);
    }

    const unsigned response = socket.ReceiveResponse();
    socket.ShutDown(ClientSocket::ShutDownOptions::SHUTDOWN_BOTH);

    return response;
}

void ReadFileBinary(
    const std::string& filePath, std::vector<char>& destination)
{
    std::ifstream sourceFileStream(filePath, std::ios::binary);

    if (!sourceFileStream)
    {
        const std::string errorMessage =
            "Cannot open file \"" + filePath + "\" for reading.";

        throw DescriptiveException(errorMessage.c_str());
    }

    destination.assign(
        std::istreambuf_iterator<char>(sourceFileStream),
        std::istreambuf_iterator<char>());
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
