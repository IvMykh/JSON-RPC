#ifndef JSON_RPC_VALIDATOR_
#define JSON_RPC_VALIDATOR_

#include "non_terminal_node.h"
#include "validation_state.h"
#include "json_rpc_object_type.h"



class JsonRpcValidator
{
public:
    JsonRpcValidator();

    const JsonRpcObjectType 
            Validate(const NonTerminalNode* root);
    
    void    Reset();

private:
    void    validateSingleObject(const NonTerminalNode* root);
    void    validateBatch(const NonTerminalNode* root);


    void    validatePair(const NonTerminalNode* pairNode);

    void    validateVersionPair(
        const Token* keyToken, const NonTerminalNode* valueNode);

    void    validateMethodPair(
        const Token* keyToken, const NonTerminalNode* valueNode);

    void    validateParamsPair(
        const Token* keyToken, const NonTerminalNode* valueNode);

    void    validateIdPair(
        const Token* keyToken, const NonTerminalNode* valueNode);

    void    validateResultPair(const Token* keyToken);

    void    validateErrorPair(
        const Token* keyToken, const NonTerminalNode* valueNode);


    void    validateErrorObject(
        const Token* keyToken, const NonTerminalNode* valueNode);

    void    validateErrorObjectPair(const NonTerminalNode* pairNode);

    void    validateErrorCodePair(
        const Token* keyToken, const NonTerminalNode* valueNode);

    void    validateErrorMessagePair(
        const Token* keyToken, const NonTerminalNode* valueNode);

    void    validateErrorDataPair(
        const Token* keyToken, const NonTerminalNode* valueNode);


    void    checkRequestForMissings(const int lineNumber) const;
    void    checkResponseForMissings(const int lineNumber) const;

private:
    ValidationState     state_;
    JsonRpcObjectType   objectType_;
};

#endif // !JSON_RPC_VALIDATOR_
