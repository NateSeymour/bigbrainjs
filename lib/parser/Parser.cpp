#include "bbjs/parser/Parser.h"

using namespace bbjs::parser;
using namespace bbjs::ast;

using G = GrammarType;

bf::DefineTerminal<G, R"(\d+(\.\d+)?)"> NUMBER([](auto const &tok) {
    return make_node<NumericLiteral>(std::stod(std::string(tok.raw)));
});

bf::DefineNonTerminal<G> bbjs::parser::root
    = NUMBER;
