#ifndef PARSER_H
#define PARSER_H

#include <buffalo/buffalo.h>
#include "Node.h"

namespace v6::parser
{
    using ValueType = ast::Node;
    using GrammarType = bf::GrammarDefinition<ValueType>;

    extern std::expected<bf::SLRParser<GrammarType>, bf::Error> Parser;
} // namespace v6::parser

#endif // PARSER_H
