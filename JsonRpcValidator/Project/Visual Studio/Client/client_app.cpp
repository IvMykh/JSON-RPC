#include "client_app.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <ctime>

#include "client_socket.h"



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
        auto bytesToSend = ReadFileBinary(sourceFilePath);
        ShowStatus("File \"" + sourceFilePath + "\" has been read successfully.", true);

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
        WSACleanup();
    }
}

const unsigned PerformCommunication(const std::vector<char>& dataToSend)
{
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 1), &wsaData) != 0)
    {
        throw ClientSocketException(
            "Initiating Winsock DLL error occurred.");
    }

    const int kPortNumber = 5000;
    const char* kServerName = "127.0.0.1";

    ClientSocket socket(kPortNumber, kServerName);

    if (!socket.IsSocketValid())
    {
        WSACleanup();

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

const std::vector<char> ReadFileBinary(const std::string& filePath)
{
    std::ifstream sourceFileStream(filePath, std::ios::binary);

    if (!sourceFileStream)
    {
        const std::string errorMessage =
            "Cannot open file \"" + filePath + "\" for reading.";

        throw std::exception(errorMessage.c_str());
    }

    return std::vector<char>(
        std::istreambuf_iterator<char>(sourceFileStream),
        std::istreambuf_iterator<char>());
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
