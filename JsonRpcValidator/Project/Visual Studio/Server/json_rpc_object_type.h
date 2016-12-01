#ifndef JSON_RPC_OBJECT_TYPE_
#define JSON_RPC_OBJECT_TYPE_

enum class JsonRpcObjectType
{
    NONE,
    REQUEST,
    RESPONSE,
    BATCH_REQUEST,
    BATCH_RESPONSE
};

#endif // !JSON_RPC_OBJECT_TYPE_