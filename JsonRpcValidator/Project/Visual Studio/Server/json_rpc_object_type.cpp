#include "json_rpc_object_type.h"



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
