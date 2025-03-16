#ifndef NODE_H
#define NODE_H

#include <variant>
#include <vector>

namespace bbjs::ast
{
    struct Empty;
    struct FunctionDefinition;
    struct NumericLiteral;

    using Node = std::variant<Empty, FunctionDefinition, NumericLiteral>;

    struct Empty
    {
    };

    struct FunctionDefinition
    {
        std::string_view name;
        std::size_t arg_count;

        std::vector<std::reference_wrapper<Node>> statements;
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

        Node &lhs;
        Operation op;
        Node &rhs;
    };

    struct Return
    {
        Node &value;
    };

    struct Export
    {
    };

    struct Import
    {
    };
} // namespace bbjs::ast

#endif // NODE_H
