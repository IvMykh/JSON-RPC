#ifndef NODE_TYPE_H_
#define NODE_TYPE_H_

enum class NodeType
{
    None,

    // Terminals
    OpenCurly,
    CloseCurly,
    OpenSquare,
    CloseSquare,
    Comma,
    Colon,
    True,
    False,
    Null,
    String,
    Number,

    // Non-terminals.
    Object,
    Pair,
    Array,
    Value
};


#endif // !NODE_TYPE_H_
