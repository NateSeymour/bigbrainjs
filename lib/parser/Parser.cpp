#include "bbjs/parser/Parser.h"

using namespace bbjs::parser;
using namespace bbjs::ast;

using G = GrammarType;

/*
 * THE TERMINALS
 */
// COMMENTS
bf::DefineTerminal<G, R"(\/\/.*$)"> SINGLE_COMMENT;

// KEYWORDS
bf::DefineTerminal<G, R"(import)"> IMPORT;
bf::DefineTerminal<G, R"(from)"> FROM;

// SYMBOLS
bf::DefineTerminal<G, R"(;)"> SEMI;
bf::DefineTerminal<G, R"(\.)"> DOT(bf::Left);
bf::DefineTerminal<G, R"(,)"> COMMA;

bf::DefineTerminal<G, R"(\{)"> CURLY_LEFT;
bf::DefineTerminal<G, R"(\})"> CURLY_RIGHT;

bf::DefineTerminal<G, R"(\()"> PAR_LEFT;
bf::DefineTerminal<G, R"(\))"> PAR_RIGHT;

bf::DefineTerminal<G, R"(\+)"> BINOP_ADD(bf::Left);
bf::DefineTerminal<G, R"(\-)"> BINOP_SUB(bf::Left);
bf::DefineTerminal<G, R"(\*)"> BINOP_MUL(bf::Left);
bf::DefineTerminal<G, R"(\/)"> BINOP_DIV(bf::Left);

bf::DefineTerminal<G, R"(&&)"> BINOP_AND(bf::Left);
bf::DefineTerminal<G, R"(\|\|)"> BINOP_OR(bf::Left);

// LITERALS
bf::DefineTerminal<G, R"(\d+(\.\d+)?)"> NUMBER([](auto const &tok) {
    return make_node<NumericLiteral>(std::stod(std::string(tok.raw)));
});

bf::DefineTerminal<G, R"("[^"]*")"> DOUBLE_STRING;
bf::DefineTerminal<G, R"('[^']*')"> SINGLE_STRING;
bf::DefineTerminal<G, R"(`[^`]*`)"> FORMAT_STRING;

// IDENTIFIERS
bf::DefineTerminal<G, R"([a-zA-Z]+)"> IDENTIFIER;

/*
 * THE GRAMMAR
 */
// FORWARD DECLS
extern bf::DefineNonTerminal<G> expression;

// LITERALS
bf::DefineNonTerminal<G> string_literal
    = bf::PR<G>(DOUBLE_STRING)
    | bf::PR<G>(SINGLE_STRING)
    | bf::PR<G>(FORMAT_STRING)
    ;

// IDENTIFIERS
bf::DefineNonTerminal<G> identifier_list
    = bf::PR<G>(IDENTIFIER)
    | (identifier_list + COMMA + IDENTIFIER)
    | (identifier_list + COMMA)
    ;

// OBJECTS
bf::DefineNonTerminal<G> destructured_object
    = (CURLY_LEFT + identifier_list + CURLY_RIGHT)
    ;

bf::DefineNonTerminal<G> object_dot_access
    = (expression + DOT + IDENTIFIER)
    ;

// FUNCTIONS
bf::DefineNonTerminal<G> argument_list
    = bf::PR<G>(expression)
    | (argument_list + COMMA + expression)
    ;

bf::DefineNonTerminal<G> function_call
    = (expression + PAR_LEFT + PAR_RIGHT)
    | (expression + PAR_LEFT + argument_list + PAR_RIGHT)
    ;

// EXPRESSIONS
bf::DefineNonTerminal<G> expression
    = bf::PR<G>(NUMBER)
    | bf::PR<G>(IDENTIFIER)
    | bf::PR<G>(object_dot_access)
    | bf::PR<G>(function_call)
    | (expression + BINOP_ADD + expression)
    | (expression + BINOP_SUB + expression)
    | (expression + BINOP_MUL + expression)
    | (expression + BINOP_DIV + expression)
    | (expression + BINOP_AND + expression)
    | (expression + BINOP_OR + expression)
    ;

bf::DefineNonTerminal<G> expression_statement
    = (expression + SEMI)
    ;

// MODULE
bf::DefineNonTerminal<G> import_target
    = bf::PR<G>(IDENTIFIER)
    | bf::PR<G>(destructured_object)
    ;

bf::DefineNonTerminal<G> import_statement
    = (IMPORT + import_target + FROM + string_literal + SEMI)
    ;

// COMMENT
bf::DefineNonTerminal<G> comment
    = bf::PR<G>(SINGLE_COMMENT)
    ;

// STATEMENT
bf::DefineNonTerminal<G> statement
    = bf::PR<G>(SEMI)
    | bf::PR<G>(comment)
    | bf::PR<G>(import_statement)
    | bf::PR<G>(expression_statement)
    ;

bf::DefineNonTerminal<G> statement_list
    = bf::PR<G>(statement)
    | (statement_list + statement)
    ;

// ROOT
bf::DefineNonTerminal<G> unscoped_block
 = bf::PR<G>(statement_list);

bf::DefineNonTerminal<G> bbjs::parser::root
    = bf::PR<G>(unscoped_block);
