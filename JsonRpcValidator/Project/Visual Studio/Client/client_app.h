#ifndef CLIENT_APP_H_
#define CLIENT_APP_H_

#include <vector>



void            RunApp();
const unsigned  PerformCommunication(const std::vector<char>& dataToSend);

const std::vector<char> 
                ReadFileBinary(const std::string& filePath);

void            ShowStatus(const std::string message, const bool shouldSeparate = false);


#endif // !CLIENT_APP_H_