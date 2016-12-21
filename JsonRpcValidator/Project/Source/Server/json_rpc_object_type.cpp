#include "json_rpc_object_type.h"



const std::string GetJsonPrcObjectTypeString(const JsonRpcObjectType type)
{
    std::string description;

    switch (type)
    {
    case JsonRpcObjectType::REQUEST:
        description = "Request"; 
        break;
    case JsonRpcObjectType::RESPONSE:
        description = "Response"; 
        break;
    case JsonRpcObjectType::BATCH_REQUEST:
        description = "Batch Request";
        break;
    case JsonRpcObjectType::BATCH_RESPONSE:
        description = "Batch Response"; 
        break;

    default: 
        description = "<unexpected>";
    }

    return description;
}
