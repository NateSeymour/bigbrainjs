#ifndef PARSER_H
#define PARSER_H

#include <buffalo/buffalo.h>
#include "Node.h"

namespace bbjs::parser
{
    using ValueType = ast::UniqueNode;
    using GrammarType = bf::GrammarDefinition<ValueType>;

    extern bf::DefineNonTerminal<GrammarType, "Root"> Root;
} // namespace bbjs::parser

#endif // PARSER_H
