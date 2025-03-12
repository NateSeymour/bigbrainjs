#ifndef NODE_H
#define NODE_H

#include <memory>
#include <variant>
#include <vector>

namespace bbjs::ast
{
    struct Empty;
    struct FunctionDefinition;

    using Node = std::variant<Empty, FunctionDefinition>;
    using UniqueNode = std::unique_ptr<Node>;

    struct Empty
    {
    };

    struct FunctionDefinition
    {
        std::string_view name;
        std::size_t arg_count;

        std::vector<UniqueNode> statements;
    };

    template<typename T>
    struct Literal
    {
        T value;
    };

    struct NumericLiteral : Literal<double>
    {
    };

    struct StringLiteral : Literal<std::string_view>
    {
    };

    struct BinaryExpression
    {
        enum class Operation
        {
            Addition,
            Subtraction,
            Multiplication,
            Division,
        };

        UniqueNode lhs;
        Operation op;
        UniqueNode rhs;
    };

    struct Return
    {
        UniqueNode value;
    };

    struct Export
    {
    };

    struct Import
    {
    };

    template<typename NodeT, typename... Args>
    UniqueNode make_node(Args... args)
    {
        return std::make_unique(Node{NodeT{std::forward<Args>(args)...}});
    }
} // namespace bbjs::ast

#endif // NODE_H
