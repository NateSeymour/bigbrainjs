#ifndef NODE_H
#define NODE_H

#include <string_view>
#include <variant>

namespace v6::parser
{
    struct NodeRef;
}

namespace v6::ast
{
    struct NodeBase;
    struct StringNode;
    struct NumericNode;
    struct ListNode;
    struct ModuleNode;

    using Node = std::variant<NodeBase, StringNode, NumericNode>;

    /**
     * Empty struct for constructing Nodes.
     */
    struct NodeBase
    {
    };

    template<typename ValueType>
    struct LiteralNode : NodeBase
    {
        ValueType value;
    };

    struct StringNode : LiteralNode<std::string_view>
    {
    };

    struct NumericNode : LiteralNode<double>
    {
    };

    struct ListNode : NodeBase
    {
    };

    struct ModuleNode : NodeBase
    {
    };

    struct FunctionDefinitionNode : NodeBase
    {
        std::string_view name;
        NodeBase &body;
    };
} // namespace v6::ast

#endif // NODE_H
