#ifndef PARSER_H
#define PARSER_H

#include <buffalo/buffalo.h>
#include "Node.h"

namespace bbjs::parser
{
    using ValueType = ast::Node;
    using GrammarType = bf::GrammarDefinition<ValueType>;

    extern std::expected<bf::SLRParser<GrammarType>, bf::Error> Parser;
} // namespace bbjs::parser

#endif // PARSER_H
