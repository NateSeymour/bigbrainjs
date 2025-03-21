#ifndef NODE_H
#define NODE_H

#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

namespace v6::ast
{
    struct NodeList;
    struct Import;
    struct Export;
    struct Statement;
    struct Declaration;
    struct Module;

    using Node = std::variant<double, std::string_view, Import, Export, Module, NodeList>;

    struct NodeList
    {
        std::vector<Node *> elements;
    };

    struct Import
    {
        std::string_view module_name;
        std::vector<std::pair<std::string_view, std::string_view>> named_imports;
    };

    struct Export
    {
    };

    struct Statement
    {
    };

    struct Declaration
    {
        bool mut = false;
    };

    struct Module
    {
        NodeList &body;
    };
} // namespace v6::ast

#endif // NODE_H
