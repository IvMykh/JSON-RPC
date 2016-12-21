#ifndef CLIENT_APP_H_
#define CLIENT_APP_H_



#include <vector>
#include <string>



void            RunApp();
const unsigned  PerformCommunication(const std::vector<char>& dataToSend);

void            ReadFileBinary(
    const std::string& filePath, std::vector<char>& destination);

void            ShowStatus(
    const std::string message, const bool shouldSeparate = false);


#endif // !CLIENT_APP_H_
