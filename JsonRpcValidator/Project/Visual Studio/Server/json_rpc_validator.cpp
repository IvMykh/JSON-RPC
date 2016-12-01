#include "json_rpc_validator.h"

#include "token.h"
#include "validation_error_exception.h"



JsonRpcValidator::JsonRpcValidator()
    : state_(ValidationState::NONE),
      objectType_(JsonRpcObjectType::NONE)
{
}

const JsonRpcObjectType JsonRpcValidator::Validate(const NonTerminalNode* root)
{
    if (root->GetType() == NodeType::Object)
    {   
        validateSingleObject(root);
    }
    else if (root->GetType() == NodeType::Array)
    {
        validateBatch(root);
    }
    else
    {
        throw ValidationErrorException(
            root->GetLineNumber(),
            "JSON-RPC must be either object or array (batch).");
    }

    return objectType_;
}

void JsonRpcValidator::Reset()
{
    state_ = ValidationState::NONE;
    objectType_ = JsonRpcObjectType::NONE;
}



void JsonRpcValidator::validateSingleObject(const NonTerminalNode* root)
{
    Reset();

    const std::list<ParseTreeNode*>& children = root->GetChildren();

    for (auto child = children.begin(); child != children.end(); ++child)
    {
        if ((*child)->GetType() == NodeType::Pair)
        {
            validatePair(dynamic_cast<NonTerminalNode*>(*child));
        }
    }

    if (objectType_ == JsonRpcObjectType::REQUEST)
    {
        checkRequestForMissings(root->GetLineNumber());
    }
    else if (objectType_ == JsonRpcObjectType::RESPONSE)
    {
        checkResponseForMissings(root->GetLineNumber());
    }
    else
    {
        throw ValidationErrorException(
            root->GetLineNumber(),
            "An object is neither valid request nor valid response.");
    }
}
void JsonRpcValidator::validateBatch(const NonTerminalNode* root)
{
    const std::list<ParseTreeNode*>& children = root->GetChildren();

    JsonRpcObjectType prevObjectType = JsonRpcObjectType::NONE;
    JsonRpcObjectType nextObjectType = JsonRpcObjectType::NONE;

    for (auto child = children.begin(); child != children.end(); ++child)
    {
        if (!((*child)->GetType() == NodeType::Comma || 
              (*child)->GetType() == NodeType::OpenSquare ||
              (*child)->GetType() == NodeType::CloseSquare))
        {
            const NonTerminalNode* valueNode = 
                dynamic_cast<NonTerminalNode*>(*child);
            
            validateSingleObject(
                dynamic_cast<NonTerminalNode*>(valueNode->GetChildren().front()));

            prevObjectType = nextObjectType;
            nextObjectType = objectType_;

            if (prevObjectType != JsonRpcObjectType::NONE && 
                nextObjectType != JsonRpcObjectType::NONE &&
                prevObjectType != nextObjectType)
            {
                throw ValidationErrorException(
                    valueNode->GetLineNumber(),
                    "All objects in Batch must be of the same type (Request or Response).");
            }
        }
    }

    if (nextObjectType == JsonRpcObjectType::REQUEST)
    {
        objectType_ = JsonRpcObjectType::BATCH_REQUEST;
    }
    else
    {
        objectType_ = JsonRpcObjectType::BATCH_RESPONSE;
    }
}

void JsonRpcValidator::validatePair(const NonTerminalNode* pairNode)
{
    auto membersIterator = pairNode->GetChildren().begin();

    Token* keyToken = dynamic_cast<Token*>(*membersIterator);
    ++membersIterator;

    // Skip ':' token.
    ++membersIterator;

    NonTerminalNode* valueNode = 
        dynamic_cast<NonTerminalNode*>(*membersIterator);
    
    if (keyToken->GetValue() == "\"jsonrpc\"")
    {
        validateVersionPair(keyToken, valueNode);
    }
    else if (keyToken->GetValue() == "\"method\"")
    {
        validateMethodPair(keyToken, valueNode);
    }
    else if (keyToken->GetValue() == "\"params\"")
    {
        validateParamsPair(keyToken, valueNode);
    }
    else if (keyToken->GetValue() == "\"id\"")
    {
        validateIdPair(keyToken, valueNode);
    }
    else if (keyToken->GetValue() == "\"result\"")
    {
        validateResultPair(keyToken);
    }
    else if (keyToken->GetValue() == "\"error\"")
    {
        validateErrorPair(keyToken, valueNode);
    }
    else
    {
        throw ValidationErrorException(
            keyToken->GetLineNumber(), "Unexpected key.");
    }
}

void JsonRpcValidator::validateVersionPair(
    const Token* keyToken, const NonTerminalNode* valueNode)
{
    if (AreFlagsSet(state_, ValidationState::VERSION_PROVIDED))
    {
        throw ValidationErrorException(
            keyToken->GetLineNumber(), 
            "JSON-RPC version is already defined.");
    }

    state_ |= ValidationState::VERSION_PROVIDED;

    Token* valueNodeChild = 
        dynamic_cast<Token*>(valueNode->GetChildren().front());

    if (valueNodeChild == nullptr || 
        valueNodeChild->GetValue() != "\"2.0\"")
    {
        throw ValidationErrorException(
            valueNode->GetLineNumber(), 
            "JSON-RPC version must be exactly \"2.0\".");
    }
}

void JsonRpcValidator::validateMethodPair(
    const Token* keyToken, const NonTerminalNode* valueNode)
{
    if (objectType_ == JsonRpcObjectType::RESPONSE)
    {
        throw ValidationErrorException(
            keyToken->GetLineNumber(),
            "Response object must not contain 'method' key.");
    }

    if (AreFlagsSet(state_, ValidationState::METHOD_PROVIDED))
    {
        throw ValidationErrorException(
            keyToken->GetLineNumber(), 
            "'method' key is already defined.");
    }

    state_ |= ValidationState::METHOD_PROVIDED;
    objectType_ = JsonRpcObjectType::REQUEST;

    Token* valueNodeChild = 
        dynamic_cast<Token*>(valueNode->GetChildren().front());

    if (valueNodeChild == nullptr || 
        valueNodeChild->GetType() != NodeType::String)
    {
        throw ValidationErrorException(
            valueNode->GetLineNumber(), 
            "Method value must be string.");
    }

    const std::string& name = valueNodeChild->GetValue();

    if (name.length() >= 4 &&
        name[0] == 'r' &&
        name[1] == 'p' &&
        name[2] == 'c' &&
        name[3] == '.')
    {
        throw ValidationErrorException(
            valueNodeChild->GetLineNumber(),
            "Method names beginning with 'rpc.' are reserved for internal use only.");
    }
}

void JsonRpcValidator::validateParamsPair(
    const Token* keyToken, const NonTerminalNode* valueNode)
{
    if (objectType_ == JsonRpcObjectType::RESPONSE)
    {
        throw ValidationErrorException(
            keyToken->GetLineNumber(),
            "Response object must not contain 'params' key.");
    }

    if (AreFlagsSet(state_, ValidationState::PARAMS_PROVIDED))
    {
        throw ValidationErrorException(
            keyToken->GetLineNumber(), 
            "'params' key is already defined.");
    }
    
    state_ |= ValidationState::PARAMS_PROVIDED;

    NonTerminalNode* valueNodeChild = 
        dynamic_cast<NonTerminalNode*>(valueNode->GetChildren().front());
    
    if (valueNodeChild == nullptr || 
        !(valueNodeChild->GetType() == NodeType::Array || 
          valueNodeChild->GetType() == NodeType::Object))
    {
        throw ValidationErrorException(
            valueNode->GetLineNumber(), 
            "'params' value must be either object or array.");
    }
}

void JsonRpcValidator::validateIdPair(
    const Token* keyToken, const NonTerminalNode* valueNode)
{
    if (AreFlagsSet(state_, ValidationState::ID_PROVIDED))
    {
        throw ValidationErrorException(
            keyToken->GetLineNumber(), 
            "'id' key is already defined.");
    }

    state_ |= ValidationState::ID_PROVIDED;

    Token* valueNodeChild = 
        dynamic_cast<Token*>(valueNode->GetChildren().front());

    if (valueNodeChild == nullptr ||
        !(valueNodeChild->GetType() == NodeType::Number ||
          valueNodeChild->GetType() == NodeType::String ||
          valueNodeChild->GetType() == NodeType::Null))
    {
        throw ValidationErrorException(
            valueNode->GetLineNumber(), 
            "'id' value must be a number, string or null.");
    }
}

void JsonRpcValidator::validateResultPair(const Token* keyToken)
{
    if (objectType_ == JsonRpcObjectType::REQUEST)
    {
        throw ValidationErrorException(
            keyToken->GetLineNumber(),
            "Request object must not contain 'result' key.");
    }

    if (AreFlagsSet(state_, ValidationState::RESULT_PROVIDED))
    {
        throw ValidationErrorException(
            keyToken->GetLineNumber(),
            "'result' key is already defined.");
    }

    if (AreFlagsSet(state_, ValidationState::ERROR_PROVIDED))
    {
        throw ValidationErrorException(
            keyToken->GetLineNumber(),
            "'error' key is already defined, 'result' cannot be defined.");
    }

    state_ |= ValidationState::RESULT_PROVIDED;
    objectType_ = JsonRpcObjectType::RESPONSE;
}

void JsonRpcValidator::validateErrorPair(
    const Token* keyToken, const NonTerminalNode* valueNode)
{
    if (objectType_ == JsonRpcObjectType::REQUEST)
    {
        throw ValidationErrorException(
            keyToken->GetLineNumber(),
            "Request object must not contain 'error' key.");
    }

    if (AreFlagsSet(state_, ValidationState::ERROR_PROVIDED))
    {
        throw ValidationErrorException(
            keyToken->GetLineNumber(),
            "'error' key is already defined.");
    }

    if (AreFlagsSet(state_, ValidationState::RESULT_PROVIDED))
    {
        throw ValidationErrorException(
            keyToken->GetLineNumber(),
            "'result' key is already defined, 'error' cannot be defined.");
    }

    state_ |= ValidationState::ERROR_PROVIDED;
    objectType_ = JsonRpcObjectType::RESPONSE;

    validateErrorObject(keyToken, valueNode);
}

void JsonRpcValidator::validateErrorObject(
    const Token* keyToken, const NonTerminalNode* valueNode)
{
    NonTerminalNode* errorObjectNode =
        dynamic_cast<NonTerminalNode*>(valueNode->GetChildren().front());

    if (errorObjectNode == nullptr || 
        errorObjectNode->GetType() != NodeType::Object)
    {
        throw ValidationErrorException(
            valueNode->GetLineNumber(), 
            "A value for 'error' must be object.");
    }

    const std::list<ParseTreeNode*>& children = errorObjectNode->GetChildren();

    for (auto child = children.begin(); child != children.end(); ++child)
    {
        if ((*child)->GetType() == NodeType::Pair)
        {
            validateErrorObjectPair(dynamic_cast<NonTerminalNode*>(*child));
        }
    }

    if (!AreFlagsSet(state_, ValidationState::ERROR_CODE_PROVIDED))
    {
        throw ValidationErrorException(
            errorObjectNode->GetLineNumber(),
            "The error object must contain 'code' key.");
    }

    if (!AreFlagsSet(state_, ValidationState::ERROR_MSG_PROVIDED))
    {
        throw ValidationErrorException(
            errorObjectNode->GetLineNumber(),
            "The error object must contain 'message' key.");
    }
}



void JsonRpcValidator::validateErrorObjectPair(const NonTerminalNode* pairNode)
{
    auto membersIterator = pairNode->GetChildren().begin();

    Token* keyToken = dynamic_cast<Token*>(*membersIterator);
    ++membersIterator;

    // Skip ':' token.
    ++membersIterator;

    NonTerminalNode* valueNode = dynamic_cast<NonTerminalNode*>(*membersIterator);


    if (keyToken->GetValue() == "\"code\"")
    {
        validateErrorCodePair(keyToken, valueNode);
    }
    else if (keyToken->GetValue() == "\"message\"")
    {
        validateErrorMessagePair(keyToken, valueNode);
    }
    else if (keyToken->GetValue() == "\"data\"")
    {
        validateErrorDataPair(keyToken, valueNode);
    }
    else
    {
        throw ValidationErrorException(
            keyToken->GetLineNumber(), "Unexpected key.");
    }
}

void JsonRpcValidator::validateErrorCodePair(
    const Token* keyToken, const NonTerminalNode* valueNode)
{
    if (AreFlagsSet(state_, ValidationState::ERROR_CODE_PROVIDED))
    {
        throw ValidationErrorException(
            keyToken->GetLineNumber(),
            "'code' key is already defined.");
    }

    state_ |= ValidationState::ERROR_CODE_PROVIDED;

    Token* valueNodeChild =
        dynamic_cast<Token*>(valueNode->GetChildren().front());

    const char* errorMessage = 
        "A value for 'code' must be an integer number.";

    if (valueNodeChild == nullptr ||
        valueNodeChild->GetType() != NodeType::Number)
    {
        throw ValidationErrorException(
            valueNode->GetLineNumber(), errorMessage);
    }

    const std::string& numberString = valueNodeChild->GetValue();
    const char firstEntry = numberString[0];

    if (!(firstEntry == '+' || firstEntry == '-' ||
          ('0' <= firstEntry && firstEntry <= '9')))
    {
        throw ValidationErrorException(
            valueNode->GetLineNumber(), errorMessage);
    }

    for (unsigned i = 1; i < numberString.length(); ++i)
    {
        if (!('0' <= numberString[i] && numberString[i] <= '9'))
        {
            throw ValidationErrorException(
                valueNode->GetLineNumber(), errorMessage);
        }
    }
}

void JsonRpcValidator::validateErrorMessagePair(
    const Token* keyToken, const NonTerminalNode* valueNode)
{
    if (AreFlagsSet(state_, ValidationState::ERROR_MSG_PROVIDED))
    {
        throw ValidationErrorException(
            keyToken->GetLineNumber(),
            "'message' key is already defined.");
    }

    state_ |= ValidationState::ERROR_MSG_PROVIDED;

    Token* valueNodeChild =
        dynamic_cast<Token*>(valueNode->GetChildren().front());

    if (valueNodeChild == nullptr ||
        valueNodeChild->GetType() != NodeType::String)
    {
        throw ValidationErrorException(
            valueNode->GetLineNumber(),
            "A value for 'message' must be string.");
    }
}

void JsonRpcValidator::validateErrorDataPair(
    const Token* keyToken, const NonTerminalNode* valueNode)
{
    if (AreFlagsSet(state_, ValidationState::ERROR_DATA_PROVIDED))
    {
        throw ValidationErrorException(
            keyToken->GetLineNumber(),
            "'data' key is already defined.");
    }

    state_ |= ValidationState::ERROR_DATA_PROVIDED;
}



void JsonRpcValidator::checkRequestForMissings(const int lineNumber) const
{
    if (!AreFlagsSet(state_, ValidationState::VERSION_PROVIDED))
    {
        throw ValidationErrorException(
            lineNumber, "'jsonrpc' key is missing.");
    }

    if (!AreFlagsSet(state_, ValidationState::METHOD_PROVIDED))
    {
        throw ValidationErrorException(
            lineNumber, "'method' key is required in Request object.");
    }
}

void JsonRpcValidator::checkResponseForMissings(const int lineNumber) const
{
    if (!AreFlagsSet(state_, ValidationState::VERSION_PROVIDED))
    {
        throw ValidationErrorException(
            lineNumber, "'jsonrpc' key is missing.");
    }

    if (!AreFlagsSet(state_, ValidationState::ID_PROVIDED))
    {
        throw ValidationErrorException(
            lineNumber, "'id' key is required in Response object.");
    }

    if (!(AreFlagsSet(state_, ValidationState::ERROR_PROVIDED) ||
          AreFlagsSet(state_, ValidationState::RESULT_PROVIDED)))
    {
        throw ValidationErrorException(
            lineNumber, 
            "There must be either 'error' or 'result' key in Response object.");
    }
}
