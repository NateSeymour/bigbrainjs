#include "bbjs/parser/Parser.h"

using namespace bbjs::parser;
using namespace bbjs::ast;

using G = GrammarType;

/*
 * THE TERMINALS
 */
// COMMENTS
bf::DefineTerminal<G, R"(\/\/.*$)"> SINGLE_COMMENT;
bf::DefineTerminal<G, R"(\/\*(.)*\*\/)"> MULTI_COMMENT;

// KEYWORDS
bf::DefineTerminal<G, R"(import)"> IMPORT;
bf::DefineTerminal<G, R"(from)"> FROM;

bf::DefineTerminal<G, R"(const)"> CONST;
bf::DefineTerminal<G, R"(let)"> LET;

bf::DefineTerminal<G, R"(for)"> FOR;
bf::DefineTerminal<G, R"(while)"> WHILE;

bf::DefineTerminal<G, R"(return)"> RETURN;

bf::DefineTerminal<G, R"(true)"> TRUE;
bf::DefineTerminal<G, R"(false)"> FALSE;

bf::DefineTerminal<G, R"(true)"> KW_NULL;
bf::DefineTerminal<G, R"(false)"> UNDEFINED;

// SYMBOLS
bf::DefineTerminal<G, R"(;)"> SEMI;
bf::DefineTerminal<G, R"(:)"> COLON;
bf::DefineTerminal<G, R"(\.)"> DOT(bf::Left);
bf::DefineTerminal<G, R"(,)"> COMMA;

bf::DefineTerminal<G, R"(\<\<\<)"> LSUS;
bf::DefineTerminal<G, R"(\>\>\>)"> RSUS;

bf::DefineTerminal<G, R"(\<\<)"> LSS;
bf::DefineTerminal<G, R"(\>\>)"> RSS;

bf::DefineTerminal<G, R"(=>)"> ARROW;

bf::DefineTerminal<G, R"(=)"> EQUAL;

bf::DefineTerminal<G, R"(\{)"> CURLY_LEFT;
bf::DefineTerminal<G, R"(\})"> CURLY_RIGHT;

bf::DefineTerminal<G, R"(\[)"> SQUARE_LEFT;
bf::DefineTerminal<G, R"(\])"> SQUARE_RIGHT;

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
extern bf::DefineNonTerminal<G> expression_list;
extern bf::DefineNonTerminal<G> scoped_block;

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

bf::DefineNonTerminal<G> object_property
    = bf::PR<G>(IDENTIFIER)
    | (IDENTIFIER + COLON + expression)
    ;

bf::DefineNonTerminal<G> object_property_list
    = bf::PR<G>(object_property)
    | (object_property_list + COMMA + object_property)
    | (object_property_list + COMMA)
    ;

bf::DefineNonTerminal<G> object
    = (CURLY_LEFT + CURLY_RIGHT)
    | (CURLY_LEFT + object_property_list + CURLY_RIGHT)
    ;

// ARRAY
bf::DefineNonTerminal<G> array
    = (SQUARE_LEFT + SQUARE_RIGHT)
    | (SQUARE_LEFT + expression_list + SQUARE_RIGHT)
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

bf::DefineNonTerminal<G> function_definition_arrow
    = (PAR_LEFT + identifier_list + PAR_RIGHT + ARROW + scoped_block)
    | (IDENTIFIER + ARROW + scoped_block)
    ;

// EXPRESSIONS
bf::DefineNonTerminal<G> expression
    = bf::PR<G>(NUMBER)
    | bf::PR<G>(IDENTIFIER)
    | bf::PR<G>(TRUE)
    | bf::PR<G>(FALSE)
    | bf::PR<G>(KW_NULL)
    | bf::PR<G>(UNDEFINED)
    | bf::PR<G>(DOUBLE_STRING)
    | bf::PR<G>(SINGLE_STRING)
    | bf::PR<G>(FORMAT_STRING)
    | bf::PR<G>(object_dot_access)
    | bf::PR<G>(function_call)
    | bf::PR<G>(function_definition_arrow)
    | bf::PR<G>(object)
    | bf::PR<G>(array)
    | (expression + BINOP_ADD + expression)
    | (expression + BINOP_SUB + expression)
    | (expression + BINOP_MUL + expression)
    | (expression + BINOP_DIV + expression)
    | (expression + BINOP_AND + expression)
    | (expression + BINOP_OR + expression)
    ;

bf::DefineNonTerminal<G> expression_list
    = bf::PR<G>(expression)
    | (expression_list + COMMA + expression)
    | (expression_list + COMMA)
    ;

bf::DefineNonTerminal<G> expression_statement
    = (expression + SEMI)
    ;

// DECLARATIONS
bf::DefineNonTerminal<G> declaration_statement
    = (CONST + IDENTIFIER + EQUAL + expression + SEMI)
    | (LET + IDENTIFIER + EQUAL + expression + SEMI)
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
    | bf::PR<G>(MULTI_COMMENT)
    ;

// RETURN
bf::DefineNonTerminal<G> return_statement
    = (RETURN + SEMI)
    | (RETURN + expression + SEMI)
    ;

// FOR LOOP
bf::DefineNonTerminal<G> for_loop
    = (FOR + PAR_LEFT + declaration_statement + SEMI + expression + SEMI + expression + PAR_RIGHT + scoped_block)
    ;

// STATEMENT
bf::DefineNonTerminal<G> statement
    = bf::PR<G>(SEMI)
    | bf::PR<G>(comment)
    | bf::PR<G>(import_statement)
    | bf::PR<G>(expression_statement)
    | bf::PR<G>(declaration_statement)
    | bf::PR<G>(return_statement)
    | bf::PR<G>(for_loop)
    ;

bf::DefineNonTerminal<G> statement_list
    = bf::PR<G>(statement)
    | (statement_list + statement)
    ;

// BLOCKS
bf::DefineNonTerminal<G> unscoped_block
    = bf::PR<G>(statement_list)
    ;

bf::DefineNonTerminal<G> scoped_block
    = (CURLY_LEFT + CURLY_RIGHT)
    | (CURLY_LEFT + statement_list + CURLY_RIGHT)
    ;

// ROOT
bf::DefineNonTerminal<G> bbjs::parser::root
    = bf::PR<G>(unscoped_block)
    ;
