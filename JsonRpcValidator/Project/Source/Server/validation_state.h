#ifndef VALIDATION_STATE_
#define VALIDATION_STATE_

#include <type_traits>



enum class ValidationState
    : short
{
    NONE                = 0,
    VERSION_PROVIDED    = 1 << 0,
    METHOD_PROVIDED     = 1 << 1,
    PARAMS_PROVIDED     = 1 << 2,
    ID_PROVIDED         = 1 << 3,
    RESULT_PROVIDED     = 1 << 4,
    ERROR_PROVIDED      = 1 << 5,

    ERROR_CODE_PROVIDED = 1 << 6,
    ERROR_MSG_PROVIDED  = 1 << 7,
    ERROR_DATA_PROVIDED = 1 << 8,
};

typedef std::underlying_type<ValidationState>::type UnderlyingType;


inline ValidationState operator | (
    const ValidationState& first, const ValidationState& second)
{
    UnderlyingType firstCasted = static_cast<UnderlyingType>(first);
    UnderlyingType secondCasted = static_cast<UnderlyingType>(second);

    return static_cast<ValidationState>(firstCasted | secondCasted);
}

inline ValidationState& operator |= (
    ValidationState& first, const ValidationState& second)
{
    return first = first | second;
}

inline bool AreFlagsSet(const ValidationState& state,  const ValidationState& flags)
{
    UnderlyingType stateCasted = static_cast<UnderlyingType>(state);
    UnderlyingType flagsCasted = static_cast<UnderlyingType>(flags);

    return (stateCasted & flagsCasted) == flagsCasted;
}

#endif // !VALIDATION_STATE_
