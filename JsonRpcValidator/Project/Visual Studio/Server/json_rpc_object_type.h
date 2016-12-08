#ifndef JSON_RPC_OBJECT_TYPE_
#define JSON_RPC_OBJECT_TYPE_

#include <string>



enum class JsonRpcObjectType
{
    NONE,
    REQUEST,
    RESPONSE,
    BATCH_REQUEST,
    BATCH_RESPONSE
};


const std::string   GetJsonPrcObjectTypeString(const JsonRpcObjectType type);

#endif // !JSON_RPC_OBJECT_TYPE_