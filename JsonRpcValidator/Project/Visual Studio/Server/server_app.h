#ifndef SERVER_APP_H_
#define SERVER_APP_H_

#include <string>
#include "json_rpc_object_type.h"



void                RunApp();
const unsigned      PerformCommunication(const unsigned& response);
const unsigned      PerformValidation(const std::string& jsonRpcText);
void                ShowStatus(const std::string message, const bool shouldSeparate = false);


#endif // !SERVER_APP_H_