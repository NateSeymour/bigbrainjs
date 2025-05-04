#include "v6turbo/parser/Parser.h"

using namespace v6;
using namespace v6::parser;

using G = GrammarType;

/*
 * THE TERMINALS
 */
// COMMENTS
bf::DefineTerminal<G, R"(\/\/.*$)"> SINGLE_COMMENT;
bf::DefineTerminal<G, R"(\/\*(.)*\*\/)"> MULTI_COMMENT;

// KEYWORDS
bf::DefineTerminal<G, R"(super)"> SUPER;
bf::DefineTerminal<G, R"(import)"> IMPORT;
bf::DefineTerminal<G, R"(export)"> EXPORT;
bf::DefineTerminal<G, R"(as)"> AS;
bf::DefineTerminal<G, R"(meta)"> META;
bf::DefineTerminal<G, R"(from)"> FROM;

bf::DefineTerminal<G, R"(async)"> ASYNC;
bf::DefineTerminal<G, R"(await)"> AWAIT;

bf::DefineTerminal<G, R"(const)"> CONST;
bf::DefineTerminal<G, R"(let)"> LET;
bf::DefineTerminal<G, R"(in)"> IN;

bf::DefineTerminal<G, R"(new)"> NEW;
bf::DefineTerminal<G, R"(target)"> TARGET;

bf::DefineTerminal<G, R"(for)"> FOR;
bf::DefineTerminal<G, R"(do)"> DO;
bf::DefineTerminal<G, R"(while)"> WHILE;
bf::DefineTerminal<G, R"(break)"> BREAK;
bf::DefineTerminal<G, R"(continue)"> CONTINUE;

bf::DefineTerminal<G, R"(if)"> IF;
bf::DefineTerminal<G, R"(else)"> ELSE;
bf::DefineTerminal<G, R"(switch)"> SWITCH;
bf::DefineTerminal<G, R"(case)"> CASE;
bf::DefineTerminal<G, R"(default)"> DEFAULT;

bf::DefineTerminal<G, R"(throw)"> THROW;
bf::DefineTerminal<G, R"(try)"> TRY;
bf::DefineTerminal<G, R"(catch)"> CATCH;
bf::DefineTerminal<G, R"(finally)"> FINALLY;
bf::DefineTerminal<G, R"(debugger)"> DEBUGGER;

bf::DefineTerminal<G, R"(with)"> WITH;

bf::DefineTerminal<G, R"(function)"> FUNCTION;
bf::DefineTerminal<G, R"(this)"> THIS;
bf::DefineTerminal<G, R"(return)"> RETURN;
bf::DefineTerminal<G, R"(yield)"> YIELD(bf::Left);

bf::DefineTerminal<G, R"(class)"> CLASS;
bf::DefineTerminal<G, R"(extends)"> EXTENDS;
bf::DefineTerminal<G, R"(static)"> STATIC;
bf::DefineTerminal<G, R"(get)"> GET;
bf::DefineTerminal<G, R"(set)"> SET;
bf::DefineTerminal<G, R"(instanceof)"> INSTANCEOF;

bf::DefineTerminal<G, R"(true)"> TRUE;
bf::DefineTerminal<G, R"(false)"> FALSE;

bf::DefineTerminal<G, R"(null)"> KW_NULL;
bf::DefineTerminal<G, R"(undefined)"> UNDEFINED;

bf::DefineTerminal<G, R"(delete)"> DELETE;
bf::DefineTerminal<G, R"(void)"> VOID;
bf::DefineTerminal<G, R"(typeof)"> TYPEOF;

// SYMBOLS
bf::DefineTerminal<G, R"(\*=)"> STAR_ASSIGN;
bf::DefineTerminal<G, R"(\/=)"> SLASH_ASSIGH;
bf::DefineTerminal<G, R"(%=)"> PERCENT_ASSIGN;
bf::DefineTerminal<G, R"(\+=)"> PLUS_ASSIGN;
bf::DefineTerminal<G, R"(\-=)"> MINUS_ASSIGN;
bf::DefineTerminal<G, R"(\<\<=)"> LSS_ASSIGN;
bf::DefineTerminal<G, R"(\>\>=)"> RSS_ASSIGN;
bf::DefineTerminal<G, R"(\>\>\>=)"> RSU_ASSIGN;
bf::DefineTerminal<G, R"(&=)"> AND_ASSIGN;
bf::DefineTerminal<G, R"(\^=)"> NOT_ASSIGN;
bf::DefineTerminal<G, R"(\|=)"> PIPE_ASSIGN;
bf::DefineTerminal<G, R"(\*\*=)"> DOUBLE_STAR_ASSIGN;

bf::DefineTerminal<G, R"(;)"> SEMI;
bf::DefineTerminal<G, R"(:)"> COLON;
bf::DefineTerminal<G, R"(\.)"> DOT(bf::Left);
bf::DefineTerminal<G, R"(\?\.)"> UNCERTAIN_DOT(bf::Left);
bf::DefineTerminal<G, R"(\?{2})"> DOUBLE_UNCERTAIN;
bf::DefineTerminal<G, R"(\?)"> UNCERTAIN;
bf::DefineTerminal<G, R"(\.{3})"> SPREAD;
bf::DefineTerminal<G, R"(,)"> COMMA;
bf::DefineTerminal<G, R"(#)"> POUND;
bf::DefineTerminal<G, R"(\+\+)"> DOUBLE_PLUS(bf::Left);
bf::DefineTerminal<G, R"(\+)"> PLUS(bf::Left);
bf::DefineTerminal<G, R"(\-\-)"> DOUBLE_MINUS(bf::Left);
bf::DefineTerminal<G, R"(\-)"> MINUS(bf::Left);
bf::DefineTerminal<G, R"(\*{2})"> DOUBLE_STAR(bf::Right);
bf::DefineTerminal<G, R"(\*)"> STAR(bf::Left);
bf::DefineTerminal<G, R"(\/)"> SLASH(bf::Left);
bf::DefineTerminal<G, R"(%)"> PERCENT(bf::Left);
bf::DefineTerminal<G, R"(!)"> EXCLAMATION;
bf::DefineTerminal<G, R"(~)"> SQUIGGLE;

bf::DefineTerminal<G, R"(=\>)"> ARROW;

bf::DefineTerminal<G, R"(===)"> TRIPLE_EQUAL;
bf::DefineTerminal<G, R"(==)"> DOUBLE_EQUAL;
bf::DefineTerminal<G, R"(=)"> EQUAL;

bf::DefineTerminal<G, R"(!=)"> NOT_EQUAL;
bf::DefineTerminal<G, R"(!==)"> NOT_DOUBLE_EQUAL;

bf::DefineTerminal<G, R"(\{)"> CURLY_LEFT;
bf::DefineTerminal<G, R"(\})"> CURLY_RIGHT;

bf::DefineTerminal<G, R"(\[)"> SQUARE_LEFT;
bf::DefineTerminal<G, R"(\])"> SQUARE_RIGHT;

bf::DefineTerminal<G, R"(\()"> PAR_LEFT;
bf::DefineTerminal<G, R"(\))"> PAR_RIGHT;

bf::DefineTerminal<G, R"(&&)"> BINOP_AND(bf::Left);
bf::DefineTerminal<G, R"(&)"> BINOP_BIT_AND(bf::Left);

bf::DefineTerminal<G, R"(\|\|)"> BINOP_OR(bf::Left);
bf::DefineTerminal<G, R"(\|)"> BINOP_BIT_OR(bf::Left);

bf::DefineTerminal<G, R"(\^)"> BINOP_BIT_XOR(bf::Left);

bf::DefineTerminal<G, R"(\>\>\>)"> RSU;
bf::DefineTerminal<G, R"(\>\>)"> RSS;

bf::DefineTerminal<G, R"(\<\<)"> LSS;

bf::DefineTerminal<G, R"(\<)"> BINCOMP_LT(bf::Left);
bf::DefineTerminal<G, R"(\>)"> BINCOMP_GT(bf::Left);

bf::DefineTerminal<G, R"(\<=)"> BINCOMP_LTE(bf::Left);
bf::DefineTerminal<G, R"(\>=)"> BINCOMP_GTE(bf::Left);

// LITERALS
bf::DefineTerminal<G, R"(0b(0|1)+)"> NUMBER_BASE2;
bf::DefineTerminal<G, R"(0o[0-7]+)"> NUMBER_BASE8;
bf::DefineTerminal<G, R"(0x[\dabcdefABCDEF]+)"> NUMBER_BASE16;
bf::DefineTerminal<G, R"(\d+(\.\d+)?)"> NUMBER_BASE10;

bf::DefineTerminal<G, R"("[^"]*")"> DOUBLE_STRING;
bf::DefineTerminal<G, R"('[^']*')"> SINGLE_STRING;
bf::DefineTerminal<G, R"(`[^`]*`)"> FORMAT_STRING;

// IDENTIFIERS
bf::DefineTerminal<G, R"([a-zA-Z$_]+[a-zA-Z$_\d]*)"> IDENTIFIER;

/*
 * THE GRAMMAR
 * Modelled as much as possible after the grammar specification in
 * https://ecma-international.org/wp-content/uploads/ECMA-262_15th_edition_june_2024.pdf
 */

extern bf::DefineNonTerminal<G, "Block"> Block;
extern bf::DefineNonTerminal<G, "Arguments"> Arguments;
extern bf::DefineNonTerminal<G, "Statement"> Statement;
extern bf::DefineNonTerminal<G, "Expression"> Expression;
extern bf::DefineNonTerminal<G, "Initializer"> Initializer;
extern bf::DefineNonTerminal<G, "StatementList"> StatementList;
extern bf::DefineNonTerminal<G, "BindingPattern"> BindingPattern;
extern bf::DefineNonTerminal<G, "UnaryExpression"> UnaryExpression;
extern bf::DefineNonTerminal<G, "MethodDefinition"> MethodDefinition;
extern bf::DefineNonTerminal<G, "ClassElementName"> ClassElementName;
extern bf::DefineNonTerminal<G, "MemberExpression"> MemberExpression;
extern bf::DefineNonTerminal<G, "CoalesceExpression"> CoalesceExpression;
extern bf::DefineNonTerminal<G, "LexicalDeclaration"> LexicalDeclaration;
extern bf::DefineNonTerminal<G, "AssignmentExpression"> AssignmentExpression;
extern bf::DefineNonTerminal<G, "LeftHandSideExpression"> LeftHandSideExpression;

bf::DefineNonTerminal<G, "Comment"> Comment
    = bf::PR<G>(SINGLE_COMMENT)
    | bf::PR<G>(MULTI_COMMENT)
    ;

bf::DefineNonTerminal<G, "Identifier"> Identifier
    = bf::PR<G>(IDENTIFIER)
    ;

bf::DefineNonTerminal<G, "PrivateIdentifier"> PrivateIdentifier
    = (POUND + Identifier)
    ;

bf::DefineNonTerminal<G, "StringLiteral"> StringLiteral
    = bf::PR<G>(DOUBLE_STRING)
    | bf::PR<G>(SINGLE_STRING)
    ;

bf::DefineNonTerminal<G, "NumericLiteral"> NumericLiteral
    = bf::PR<G>(NUMBER_BASE2)
    | bf::PR<G>(NUMBER_BASE8)
    | bf::PR<G>(NUMBER_BASE10)
    | bf::PR<G>(NUMBER_BASE16)
    ;

bf::DefineNonTerminal<G, "NullLiteral"> NullLiteral
    = bf::PR<G>(KW_NULL)
    ;

bf::DefineNonTerminal<G, "BooleanLiteral"> BooleanLiteral
    = bf::PR<G>(TRUE)
    | bf::PR<G>(FALSE)
    ;

bf::DefineNonTerminal<G, "Literal"> Literal
    = bf::PR<G>(StringLiteral)
    | bf::PR<G>(NumericLiteral)
    | bf::PR<G>(NullLiteral)
    | bf::PR<G>(BooleanLiteral)
    ;

bf::DefineNonTerminal<G, "Elision"> Elision
    = bf::PR<G>(COMMA)
    | (Elision + COMMA)
    ;

bf::DefineNonTerminal<G, "SpreadElement"> SpreadElement
    = (SPREAD + AssignmentExpression)
    ;

bf::DefineNonTerminal<G, "ElementList"> ElementList
    = bf::PR<G>(AssignmentExpression)
    | bf::PR<G>(SpreadElement)
    | (Elision + AssignmentExpression)
    | (Elision + SpreadElement)
    | (ElementList + COMMA + AssignmentExpression)
    | (ElementList + COMMA + SpreadElement)
    | (ElementList + COMMA + Elision + AssignmentExpression)
    | (ElementList + COMMA + Elision + SpreadElement)
    ;

bf::DefineNonTerminal<G, "ArrayLiteral"> ArrayLiteral
    = (SQUARE_LEFT + SQUARE_RIGHT)
    | (SQUARE_LEFT + Elision + SQUARE_RIGHT)
    | (SQUARE_LEFT + ElementList + SQUARE_RIGHT)
    | (SQUARE_LEFT + ElementList  + COMMA + SQUARE_RIGHT)
    | (SQUARE_LEFT + ElementList  + COMMA + Elision + SQUARE_RIGHT)
    ;

bf::DefineNonTerminal<G, "ComputedPropertyName"> ComputedPropertyName
    = (SQUARE_LEFT + AssignmentExpression + SQUARE_RIGHT)
    ;

bf::DefineNonTerminal<G, "PropertyName"> PropertyName
    = bf::PR<G>(Identifier)
    | bf::PR<G>(StringLiteral)
    | bf::PR<G>(NumericLiteral)
    | bf::PR<G>(ComputedPropertyName)
    ;

bf::DefineNonTerminal<G, "CoverInitializedName"> CoverInitializedName
    = (Identifier + Initializer)
    ;

bf::DefineNonTerminal<G, "PropertyDefinition"> PropertyDefinition
    = bf::PR<G>(Identifier)
    | bf::PR<G>(CoverInitializedName)
    | (PropertyName + COLON + AssignmentExpression)
    | bf::PR<G>(MethodDefinition)
    | (SPREAD + AssignmentExpression)
    ;

bf::DefineNonTerminal<G, "PropertyDefinitionList"> PropertyDefinitionList
    = bf::PR<G>(PropertyDefinition)
    | (PropertyDefinitionList + COMMA + PropertyDefinition)
    ;

bf::DefineNonTerminal<G, "ObjectLiteral"> ObjectLiteral
    = (CURLY_LEFT + CURLY_RIGHT)
    | (CURLY_LEFT + PropertyDefinitionList + CURLY_RIGHT)
    | (CURLY_LEFT + PropertyDefinitionList + COMMA + CURLY_RIGHT)
    ;

// TODO: Implement proper template parsing
bf::DefineNonTerminal<G, "TemplateLiteral"> TemplateLiteral
    = bf::PR<G>(FORMAT_STRING)
    ;

// TODO: Implement RegularExpressionLiteral
// extern bf::DefineNonTerminal<G, "RegularExpressionLiteral"> RegularExpressionLiteral;

bf::DefineNonTerminal<G, "FormalParameter"> FormalParameter
    = bf::PR<G>(Identifier)
    ;

bf::DefineNonTerminal<G, "FormalParameterList"> FormalParameterList
    = bf::PR<G>(FormalParameter)
    | (FormalParameterList + COMMA + FormalParameter)
    ;

bf::DefineNonTerminal<G, "FormalParameters"> FormalParameters
    = bf::PR<G>(FormalParameterList)
    ;

bf::DefineNonTerminal<G, "UniqueFormalParameters"> UniqueFormalParameters
    = bf::PR<G>(FormalParameters)
    ;

bf::DefineNonTerminal<G, "FunctionExpression"> FunctionExpression
    // Parameterless Named Function
    = (FUNCTION + Identifier + PAR_LEFT + PAR_RIGHT + Block)

    // Parametrized Named Function
    | (FUNCTION + Identifier + PAR_LEFT + FormalParameters + PAR_RIGHT + Block)

    // Parameterless Anonymous Function
    | (FUNCTION + PAR_LEFT + PAR_RIGHT + Block)

    // Parametrized Anonymous Function
    | (FUNCTION + PAR_LEFT + FormalParameters + PAR_RIGHT + Block)
    ;

bf::DefineNonTerminal<G, "AsyncGeneratorMethod"> AsyncGeneratorMethod
    = (ASYNC + STAR + ClassElementName + PAR_LEFT + UniqueFormalParameters + PAR_RIGHT + Block)
    ;

bf::DefineNonTerminal<G, "AsyncGeneratorExpression"> AsyncGeneratorExpression
    = (ASYNC + FUNCTION + STAR + PAR_LEFT + FormalParameters + PAR_RIGHT + Block)
    | (ASYNC + FUNCTION + STAR + Identifier + PAR_LEFT + FormalParameters + PAR_RIGHT + Block)
    ;

bf::DefineNonTerminal<G, "AsyncMethod"> AsyncMethod
    = (ASYNC + ClassElementName + PAR_LEFT + UniqueFormalParameters + PAR_RIGHT + Block)
    ;

bf::DefineNonTerminal<G, "AsyncFunctionExpression"> AsyncFunctionExpression
    = (ASYNC + FUNCTION + PAR_LEFT + FormalParameters + PAR_RIGHT + Block)
    | (ASYNC + FUNCTION + Identifier + PAR_LEFT + FormalParameters + PAR_RIGHT + Block)
    ;

bf::DefineNonTerminal<G, "GeneratorMethod"> GeneratorMethod
    = (STAR + ClassElementName + PAR_LEFT + UniqueFormalParameters + PAR_RIGHT + Block)
    ;

bf::DefineNonTerminal<G, "GeneratorExpression"> GeneratorExpression
    = (FUNCTION + STAR + PAR_LEFT + FormalParameters + PAR_RIGHT + Block)
    | (FUNCTION + STAR + Identifier + PAR_LEFT + FormalParameters + PAR_RIGHT + Block)
    ;

bf::DefineNonTerminal<G, "ClassStaticBlockStatementList"> ClassStaticBlockStatementList
    = bf::PR<G>(StatementList)
    ;

bf::DefineNonTerminal<G, "ClassStaticBlockBody"> ClassStaticBlockBody
    = bf::PR<G>(ClassStaticBlockStatementList)
    ;

bf::DefineNonTerminal<G, "ClassStaticBlock"> ClassStaticBlock
    = (STATIC + CURLY_LEFT + CURLY_RIGHT)
    | (STATIC + CURLY_LEFT + ClassStaticBlockBody + CURLY_RIGHT)
    ;

bf::DefineNonTerminal<G, "ClassElementName"> ClassElementName
    = bf::PR<G>(PropertyName)
    | bf::PR<G>(PrivateIdentifier)
    ;

bf::DefineNonTerminal<G, "PropertySetParameterList"> PropertySetParameterList
    = bf::PR<G>(FormalParameter)
    ;

bf::DefineNonTerminal<G, "MethodDefinition"> MethodDefinition
    = (ClassElementName + PAR_LEFT + UniqueFormalParameters + PAR_RIGHT + Block)
    | bf::PR<G>(GeneratorMethod)
    | bf::PR<G>(AsyncMethod)
    | bf::PR<G>(AsyncGeneratorMethod)
    | (GET + ClassElementName + PAR_LEFT + PAR_RIGHT + Block)
    | (SET + ClassElementName + PAR_LEFT + PropertySetParameterList + PAR_RIGHT + Block)
    ;

bf::DefineNonTerminal<G, "Initializer"> Initializer
    = (EQUAL + AssignmentExpression)
    ;

bf::DefineNonTerminal<G, "FieldDefinition"> FieldDefinition
    = (ClassElementName + Initializer)
    | bf::PR<G>(ClassElementName)
    ;

bf::DefineNonTerminal<G, "ClassElement"> ClassElement
    = bf::PR<G>(MethodDefinition)
    | (STATIC + MethodDefinition)
    | (FieldDefinition + SEMI)
    | (STATIC + FieldDefinition + SEMI)
    | bf::PR<G>(ClassStaticBlock)
    | bf::PR<G>(SEMI)
    ;

bf::DefineNonTerminal<G, "ClassElementList"> ClassElementList
    = bf::PR<G>(ClassElement)
    | (ClassElementList + ClassElement)
    ;

bf::DefineNonTerminal<G, "ClassBody"> ClassBody
    = bf::PR<G>(ClassElementList)
    ;

bf::DefineNonTerminal<G, "ClassHeritage"> ClassHeritage
    = (EXTENDS + LeftHandSideExpression)
    ;

bf::DefineNonTerminal<G, "ClassTail"> ClassTail
    = (CURLY_LEFT + CURLY_RIGHT)
    | (CURLY_LEFT + ClassBody + CURLY_RIGHT)
    | (ClassHeritage + CURLY_LEFT + CURLY_RIGHT)
    | (ClassHeritage + CURLY_LEFT + ClassBody + CURLY_RIGHT)
    ;

bf::DefineNonTerminal<G, "ClassExpression"> ClassExpression
    = (CLASS + Identifier + ClassTail)
    | (CLASS + ClassTail)
    ;

bf::DefineNonTerminal<G, "BindingRestProperty"> BindingRestProperty
    = (SPREAD + Identifier)
    ;

bf::DefineNonTerminal<G, "SingleNameBinding"> SingleNameBinding
    = bf::PR<G>(Identifier)
    | (Identifier + Initializer)
    ;

bf::DefineNonTerminal<G, "BindingElement"> BindingElement
    = bf::PR<G>(SingleNameBinding)
    | bf::PR<G>(BindingPattern)
    | bf::PR<G>(BindingPattern + Initializer)
    ;

bf::DefineNonTerminal<G, "BindingProperty"> BindingProperty
    = bf::PR<G>(SingleNameBinding)
    | (PropertyName + COLON + BindingElement)
    ;

bf::DefineNonTerminal<G, "BindingPropertyList"> BindingPropertyList
    = bf::PR<G>(BindingProperty)
    | (BindingPropertyList + COMMA + BindingProperty)
    ;

bf::DefineNonTerminal<G, "ObjectBindingPattern"> ObjectBindingPattern
    = (CURLY_LEFT + CURLY_RIGHT)
    | (CURLY_LEFT + BindingRestProperty + CURLY_RIGHT)
    | (CURLY_LEFT + BindingPropertyList + CURLY_RIGHT)
    | (CURLY_LEFT + BindingPropertyList + COMMA + CURLY_RIGHT)
    | (CURLY_LEFT + BindingPropertyList + COMMA + BindingRestProperty + CURLY_RIGHT)
    ;

bf::DefineNonTerminal<G, "BindingElisionElement"> BindingElisionElement
    = bf::PR<G>(BindingElement)
    | (Elision + BindingElement)
    ;

bf::DefineNonTerminal<G, "BindingElementList"> BindingElementList
    = bf::PR<G>(BindingElisionElement)
    | (BindingElementList + COMMA + BindingElisionElement)
    ;

bf::DefineNonTerminal<G, "BindingRestElement"> BindingRestElement
    = (SPREAD + Identifier)
    | (SPREAD + BindingPattern)
    ;

bf::DefineNonTerminal<G, "ArrayBindingPattern"> ArrayBindingPattern
    = (SQUARE_LEFT + SQUARE_RIGHT)
    | (SQUARE_LEFT + Elision + SQUARE_RIGHT)
    | (SQUARE_LEFT + BindingRestElement + SQUARE_RIGHT)
    | (SQUARE_LEFT + Elision + BindingRestElement + SQUARE_RIGHT)
    | (SQUARE_LEFT + BindingElementList + SQUARE_RIGHT)
    | (SQUARE_LEFT + BindingElementList + COMMA + SQUARE_RIGHT)
    | (SQUARE_LEFT + BindingElementList + COMMA + Elision + SQUARE_RIGHT)
    | (SQUARE_LEFT + BindingElementList + COMMA + BindingRestElement + SQUARE_RIGHT)
    | (SQUARE_LEFT + BindingElementList + COMMA + Elision + BindingRestElement + SQUARE_RIGHT)
    ;

bf::DefineNonTerminal<G, "BindingPattern"> BindingPattern
    = bf::PR<G>(ObjectBindingPattern)
    | bf::PR<G>(ArrayBindingPattern)
    ;

bf::DefineNonTerminal<G, "CoverParenthesizedExpressionAndArrowParameterList"> CoverParenthesizedExpressionAndArrowParameterList
    = (PAR_LEFT + Expression + PAR_RIGHT)
    | (PAR_LEFT + Expression + COMMA + PAR_RIGHT)
    | (PAR_LEFT + PAR_RIGHT)
    | (PAR_LEFT + SPREAD + Identifier + PAR_RIGHT)
    | (PAR_LEFT + SPREAD + BindingPattern + PAR_RIGHT)
    | (PAR_LEFT + Expression + COMMA + SPREAD + Identifier + PAR_RIGHT)
    | (PAR_LEFT + Expression + COMMA + SPREAD + BindingPattern + PAR_RIGHT)
    ;

bf::DefineNonTerminal<G, "PrimaryExpression"> PrimaryExpression
    = bf::PR<G>(THIS)
    | bf::PR<G>(Identifier)
    | bf::PR<G>(Literal)
    | bf::PR<G>(ArrayLiteral)
    | bf::PR<G>(ObjectLiteral)
    //| bf::PR<G>(FunctionExpression)
    //| bf::PR<G>(ClassExpression)
    //| bf::PR<G>(GeneratorExpression)
    //| bf::PR<G>(AsyncFunctionExpression)
    //| bf::PR<G>(AsyncGeneratorExpression)
    // | bf::PR<G>(RegularExpressionLiteral)
    | bf::PR<G>(TemplateLiteral)
    | bf::PR<G>(CoverParenthesizedExpressionAndArrowParameterList)
    ;

bf::DefineNonTerminal<G, "UpdateExpression"> UpdateExpression
    = bf::PR<G>(LeftHandSideExpression)
    | (LeftHandSideExpression + DOUBLE_PLUS)
    | (LeftHandSideExpression + DOUBLE_MINUS)
    | (DOUBLE_PLUS + UnaryExpression)
    | (DOUBLE_MINUS + UnaryExpression)
    ;

bf::DefineNonTerminal<G, "AwaitExpression"> AwaitExpression
    = (AWAIT + UnaryExpression)
    ;

bf::DefineNonTerminal<G, "UnaryExpression"> UnaryExpression
    = bf::PR<G>(UpdateExpression)
    | (DELETE + UpdateExpression)
    | (VOID + UpdateExpression)
    | (TYPEOF + UpdateExpression)
    | (PLUS + UpdateExpression)
    | (MINUS + UpdateExpression)
    | (SQUIGGLE + UpdateExpression)
    | (EXCLAMATION + UpdateExpression)
    | bf::PR<G>(AwaitExpression)
    ;

bf::DefineNonTerminal<G, "ExponentiationExpression"> ExponentiationExpression
    = bf::PR<G>(UnaryExpression)
    | (UpdateExpression + DOUBLE_STAR + ExponentiationExpression)
    ;

bf::DefineNonTerminal<G, "MultiplicativeExpression"> MultiplicativeExpression
    = bf::PR<G>(ExponentiationExpression)
    | (MultiplicativeExpression + STAR + ExponentiationExpression)
    | (MultiplicativeExpression + SLASH + ExponentiationExpression)
    | (MultiplicativeExpression + PERCENT + ExponentiationExpression)
    ;

bf::DefineNonTerminal<G, "AdditiveExpression"> AdditiveExpression
    = bf::PR<G>(MultiplicativeExpression)
    | (AdditiveExpression + PLUS + MultiplicativeExpression)
    | (AdditiveExpression + MINUS + MultiplicativeExpression)
    ;

bf::DefineNonTerminal<G, "ShiftExpression"> ShiftExpression
    = bf::PR<G>(AdditiveExpression)
    | (ShiftExpression + LSS + AdditiveExpression)
    | (ShiftExpression + RSS + AdditiveExpression)
    | (ShiftExpression + RSU + AdditiveExpression)
    ;

bf::DefineNonTerminal<G, "RelationalExpression"> RelationalExpression
    = bf::PR<G>(ShiftExpression)
    | (RelationalExpression + BINCOMP_LT + ShiftExpression)
    | (RelationalExpression + BINCOMP_GT + ShiftExpression)
    | (RelationalExpression + BINCOMP_LTE + ShiftExpression)
    | (RelationalExpression + BINCOMP_GTE + ShiftExpression)
    | (RelationalExpression + INSTANCEOF + ShiftExpression)
    | (RelationalExpression + IN + ShiftExpression)
    | (PrivateIdentifier + IN + ShiftExpression)
    ;

bf::DefineNonTerminal<G, "EqualityExpression"> EqualityExpression
    = bf::PR<G>(RelationalExpression)
    | (EqualityExpression + DOUBLE_EQUAL + RelationalExpression)
    | (EqualityExpression + NOT_EQUAL + RelationalExpression)
    | (EqualityExpression + TRIPLE_EQUAL + RelationalExpression)
    | (EqualityExpression + NOT_DOUBLE_EQUAL + RelationalExpression)
    ;

bf::DefineNonTerminal<G, "BitwiseANDExpression"> BitwiseANDExpression
    = bf::PR<G>(EqualityExpression)
    | (BitwiseANDExpression + BINOP_BIT_AND + EqualityExpression)
    ;

bf::DefineNonTerminal<G, "BitwiseXORExpression"> BitwiseXORExpression
    = bf::PR<G>(BitwiseANDExpression)
    | bf::PR<G>(BitwiseXORExpression + BINOP_BIT_XOR + BitwiseANDExpression)
    ;

bf::DefineNonTerminal<G, "BitwiseORExpression"> BitwiseORExpression
    = bf::PR<G>(BitwiseXORExpression)
    | (BitwiseORExpression + BINOP_BIT_OR + BitwiseXORExpression)
    ;

bf::DefineNonTerminal<G, "CoalesceExpressionHead"> CoalesceExpressionHead
    = bf::PR<G>(CoalesceExpression)
    | bf::PR<G>(BitwiseORExpression)
    ;

bf::DefineNonTerminal<G, "CoalesceExpression"> CoalesceExpression
    = (CoalesceExpressionHead + DOUBLE_UNCERTAIN + BitwiseORExpression)
    ;

bf::DefineNonTerminal<G, "LogicalANDExpression"> LogicalANDExpression
    = bf::PR<G>(BitwiseORExpression)
    | (LogicalANDExpression + BINOP_AND + BitwiseORExpression)
    ;

bf::DefineNonTerminal<G, "LogicalORExpression"> LogicalORExpression
    = bf::PR<G>(LogicalANDExpression)
    | (LogicalORExpression + BINOP_OR + LogicalANDExpression)
    ;

bf::DefineNonTerminal<G, "ShortCircuitExpression"> ShortCircuitExpression
    = bf::PR<G>(LogicalORExpression)
    | bf::PR<G>(CoalesceExpression)
    ;

bf::DefineNonTerminal<G, "ConditionalExpression"> ConditionalExpression
    = bf::PR<G>(ShortCircuitExpression)
    | (ShortCircuitExpression + UNCERTAIN + AssignmentExpression + COLON + AssignmentExpression)
    ;

bf::DefineNonTerminal<G, "YieldExpression"> YieldExpression
    = bf::PR<G>(YIELD)
    | (YIELD + AssignmentExpression)
    | (YIELD + STAR + AssignmentExpression)
    ;

bf::DefineNonTerminal<G, "ArrowParameters"> ArrowParameters
    = bf::PR<G>(Identifier)
    | bf::PR<G>(CoverParenthesizedExpressionAndArrowParameterList)
    ;

bf::DefineNonTerminal<G, "ExpressionBody"> ExpressionBody
    = bf::PR<G>(AssignmentExpression)
    ;

bf::DefineNonTerminal<G, "ConciseBody"> ConciseBody
    = bf::PR<G>(ExpressionBody).Short(CURLY_LEFT)
    | bf::PR<G>(Block)
    ;

bf::DefineNonTerminal<G, "ArrowFunction"> ArrowFunction
    = (ArrowParameters + ARROW + ConciseBody)
    ;

bf::DefineNonTerminal<G, "CoverCallExpressionAndAsyncArrowHead"> CoverCallExpressionAndAsyncArrowHead
    = (MemberExpression + Arguments)
    ;

bf::DefineNonTerminal<G, "AsyncArrowFunction"> AsyncArrowFunction
    = (ASYNC + ArrowParameters + ARROW + ConciseBody)
    ;

bf::DefineNonTerminal<G, "SuperProperty"> SuperProperty
    = (SUPER + SQUARE_LEFT + Expression + SQUARE_RIGHT)
    | (SUPER + DOT + Identifier)
    ;

bf::DefineNonTerminal<G, "NewTarget"> NewTarget
    = (NEW + DOT + TARGET)
    ;

bf::DefineNonTerminal<G, "ImportMeta"> ImportMeta
    = (IMPORT + DOT + META)
    ;

bf::DefineNonTerminal<G, "MetaProperty"> MetaProperty
    = bf::PR<G>(NewTarget)
    | bf::PR<G>(ImportMeta)
    ;

bf::DefineNonTerminal<G, "ArgumentList"> ArgumentList
    = bf::PR<G>(AssignmentExpression)
    | (SPREAD + AssignmentExpression)
    | (ArgumentList + COMMA + AssignmentExpression)
    | (ArgumentList + COMMA + SPREAD + AssignmentExpression)
    ;

bf::DefineNonTerminal<G, "Arguments"> Arguments
    = (PAR_LEFT + PAR_RIGHT)
    | (PAR_LEFT + ArgumentList + PAR_RIGHT)
    | (PAR_LEFT + ArgumentList + COMMA + PAR_RIGHT)
    ;

bf::DefineNonTerminal<G, "MemberExpression"> MemberExpression
    = bf::PR<G>(PrimaryExpression)
    | (MemberExpression + SQUARE_LEFT + Expression + SQUARE_RIGHT)
    | (MemberExpression + DOT + Identifier)
    | (MemberExpression + TemplateLiteral)
    | bf::PR<G>(SuperProperty)
    | bf::PR<G>(MetaProperty)
    | (NEW + MemberExpression + Arguments)
    ;

bf::DefineNonTerminal<G, "NewExpression"> NewExpression
    = bf::PR<G>(MemberExpression)
    | (NEW + NewExpression)
    ;

bf::DefineNonTerminal<G, "SuperCall"> SuperCall
    = (SUPER + Arguments)
    ;

bf::DefineNonTerminal<G, "ImportCall"> ImportCall
    = (IMPORT + PAR_LEFT + AssignmentExpression + PAR_RIGHT)
    ;

bf::DefineNonTerminal<G, "CallExpression"> CallExpression
    = bf::PR<G>(CoverCallExpressionAndAsyncArrowHead)
    | bf::PR<G>(SuperCall)
    | bf::PR<G>(ImportCall)
    | (CallExpression + Arguments)
    | (CallExpression + SQUARE_LEFT + Expression + SQUARE_RIGHT)
    | (CallExpression + DOT + Identifier)
    | (CallExpression + TemplateLiteral)
    ;

bf::DefineNonTerminal<G, "OptionalChain"> OptionalChain
    = (UNCERTAIN_DOT + Arguments)
    | (UNCERTAIN_DOT + SQUARE_LEFT + Expression + SQUARE_RIGHT)
    | (UNCERTAIN_DOT + Identifier)
    | (UNCERTAIN_DOT + TemplateLiteral)
    | (OptionalChain + Arguments)
    | (OptionalChain + SQUARE_LEFT + Expression + SQUARE_RIGHT)
    | (OptionalChain + DOT + Identifier)
    | (OptionalChain + TemplateLiteral)
    ;

bf::DefineNonTerminal<G, "OptionalExpression"> OptionalExpression
    = (MemberExpression + OptionalChain)
    | (CallExpression + OptionalChain)
    | (OptionalExpression + OptionalChain)
    ;

bf::DefineNonTerminal<G, "LeftHandSideExpression"> LeftHandSideExpression
    = bf::PR<G>(NewExpression)
    | bf::PR<G>(CallExpression)
    | bf::PR<G>(OptionalExpression)
    ;

bf::DefineNonTerminal<G, "AssignementOperator"> AssignmentOperator
    = bf::PR<G>(STAR_ASSIGN)
    | bf::PR<G>(SLASH_ASSIGH)
    | bf::PR<G>(PERCENT_ASSIGN)
    | bf::PR<G>(PLUS_ASSIGN)
    | bf::PR<G>(MINUS_ASSIGN)
    | bf::PR<G>(LSS_ASSIGN)
    | bf::PR<G>(RSS_ASSIGN)
    | bf::PR<G>(RSU_ASSIGN)
    | bf::PR<G>(AND_ASSIGN)
    | bf::PR<G>(NOT_ASSIGN)
    | bf::PR<G>(PIPE_ASSIGN)
    | bf::PR<G>(DOUBLE_STAR_ASSIGN)
    ;

bf::DefineNonTerminal<G, "AssignmentExpression"> AssignmentExpression
    = bf::PR<G>(ConditionalExpression)
    | bf::PR<G>(YieldExpression)
    | bf::PR<G>(ArrowFunction)
    | bf::PR<G>(AsyncArrowFunction)
    | (LeftHandSideExpression + EQUAL + AssignmentExpression)
    | (LeftHandSideExpression + AssignmentOperator + AssignmentExpression)
    ;

bf::DefineNonTerminal<G, "Expression"> Expression
    = bf::PR<G>(AssignmentExpression)
    | (Expression + COMMA + AssignmentExpression)
    ;

bf::DefineNonTerminal<G, "BlockStatement"> BlockStatement
    = bf::PR<G>(Block)
    ;

bf::DefineNonTerminal<G, "EmptyStatement"> EmptyStatement
    = bf::PR<G>(SEMI)
    ;

bf::DefineNonTerminal<G, "ExpressionStatement"> ExpressionStatement
    = (Expression + SEMI)
    ;

bf::DefineNonTerminal<G, "IfStatement"> IfStatement
    = (IF + PAR_LEFT + Expression + PAR_RIGHT + Statement)
    | (IF + PAR_LEFT + Expression + PAR_RIGHT + Statement + ELSE + Statement)
    ;

bf::DefineNonTerminal<G, "DoWhileStatement"> DoWhileStatement
    = (DO + Statement + WHILE + PAR_LEFT + Expression + PAR_RIGHT + SEMI)
    ;

bf::DefineNonTerminal<G, "WhileStatement"> WhileStatement
    = (WHILE + PAR_LEFT + Expression + PAR_RIGHT + Statement)
    ;

bf::DefineNonTerminal<G, "ForStatement"> ForStatement
    // Oh, how nice it would be if `buffalo` supported OPTIONALs...
    = (FOR + PAR_LEFT + SEMI + SEMI + PAR_RIGHT + Statement)
    | (FOR + PAR_LEFT + Expression + SEMI + SEMI + PAR_RIGHT + Statement)
    | (FOR + PAR_LEFT + Expression + SEMI + Expression + SEMI + PAR_RIGHT + Statement)
    | (FOR + PAR_LEFT + Expression + SEMI + Expression + SEMI + Expression + PAR_RIGHT + Statement)
    | (FOR + PAR_LEFT + SEMI + Expression + SEMI + PAR_RIGHT + Statement)
    | (FOR + PAR_LEFT + SEMI + Expression + SEMI + Expression + PAR_RIGHT + Statement)
    | (FOR + PAR_LEFT + SEMI + SEMI + Expression + PAR_RIGHT + Statement)
    | (FOR + PAR_LEFT + Expression + SEMI + SEMI + Expression + PAR_RIGHT + Statement)

    | (FOR + PAR_LEFT + LexicalDeclaration + SEMI + PAR_RIGHT + Statement)
    | (FOR + PAR_LEFT + LexicalDeclaration + Expression + SEMI + PAR_RIGHT + Statement)
    | (FOR + PAR_LEFT + LexicalDeclaration + Expression + SEMI + Expression + PAR_RIGHT + Statement)
    | (FOR + PAR_LEFT + LexicalDeclaration + SEMI + Expression + PAR_RIGHT + Statement)
    ;

// TODO: Implement for (in/of)
// extern bf::DefineNonTerminal<G, "ForInOfStatement"> ForInOfStatement;

bf::DefineNonTerminal<G, "IterationStatement"> IterationStatement
    = bf::PR<G>(DoWhileStatement)
    | bf::PR<G>(WhileStatement)
    | bf::PR<G>(ForStatement)
    // | bf::PR<G>(ForInOfStatement)
    ;

bf::DefineNonTerminal<G, "DefaultClause"> DefaultClause
    = (DEFAULT + COLON + StatementList)
    | (DEFAULT + COLON)
    ;

bf::DefineNonTerminal<G, "CaseClause"> CaseClause
    = (CASE + Expression + COLON + StatementList)
    | (CASE + Expression + COLON)
    ;

bf::DefineNonTerminal<G, "CaseClauses"> CaseClauses
    = bf::PR<G>(CaseClause)
    | (CaseClauses + CaseClause)
    ;

bf::DefineNonTerminal<G, "CaseBlock"> CaseBlock
    = (CURLY_LEFT + CURLY_RIGHT)
    | (CURLY_LEFT + CaseClauses + CURLY_RIGHT)
    | (CURLY_LEFT + CaseClauses + DefaultClause + CURLY_RIGHT)
    | (CURLY_LEFT + CaseClauses + DefaultClause + CaseClauses + CURLY_RIGHT)
    | (CURLY_LEFT + DefaultClause + CURLY_RIGHT)
    | (CURLY_LEFT + DefaultClause + CaseClauses + CURLY_RIGHT)
    ;

bf::DefineNonTerminal<G, "SwitchStatement"> SwitchStatement
    = (SWITCH + PAR_LEFT + Expression + PAR_RIGHT + CaseBlock)
    ;

bf::DefineNonTerminal<G, "BreakableStatement"> BreakableStatement
    = bf::PR<G>(IterationStatement)
    | bf::PR<G>(SwitchStatement)
    ;

bf::DefineNonTerminal<G, "ContinueStatement"> ContinueStatement
    = (CONTINUE + SEMI)
    | (CONTINUE + Identifier + SEMI)
    ;

bf::DefineNonTerminal<G, "BreakStatement"> BreakStatement
    = (BREAK + COLON)
    | (BREAK + Identifier + SEMI)
    ;

bf::DefineNonTerminal<G, "ReturnStatement"> ReturnStatement
    = (RETURN + SEMI)
    | (RETURN + Expression + SEMI)
    ;

bf::DefineNonTerminal<G, "WithStatement"> WithStatement
    = (WITH + PAR_LEFT + Expression + PAR_RIGHT + Expression)
    ;

bf::DefineNonTerminal<G, "LabelledStatement"> LabelledStatement
    = (Identifier + COLON + Statement)
    ;

bf::DefineNonTerminal<G, "ThrowStatement"> ThrowStatement
    = (THROW + Expression)
    ;

bf::DefineNonTerminal<G, "CatchParameter"> CatchParameter
    = bf::PR<G>(Identifier)
    | bf::PR<G>(BindingPattern)
    ;

bf::DefineNonTerminal<G, "Catch"> Catch
    = (CATCH + PAR_LEFT + CatchParameter + PAR_RIGHT + Block)
    | (CATCH + Block)
    ;

bf::DefineNonTerminal<G, "Finally"> Finally
    = (FINALLY + Block)
    ;

bf::DefineNonTerminal<G, "TryStatement"> TryStatement
    = (TRY + Block + Catch)
    | (TRY + Block + Finally)
    | (TRY + Block + Catch + Finally)
    ;

bf::DefineNonTerminal<G, "DebuggerStatement"> DebuggerStatement
    = (DEBUGGER + SEMI)
    ;

bf::DefineNonTerminal<G, "Statement"> Statement
    = bf::PR<G>(Comment)
    | bf::PR<G>(BlockStatement)
    | bf::PR<G>(EmptyStatement)
    | bf::PR<G>(ExpressionStatement).Short(CURLY_LEFT)
    | bf::PR<G>(IfStatement)
    | bf::PR<G>(BreakableStatement)
    | bf::PR<G>(ContinueStatement)
    | bf::PR<G>(BreakStatement)
    | bf::PR<G>(ReturnStatement)
    | bf::PR<G>(WithStatement)
    | bf::PR<G>(LabelledStatement)
    | bf::PR<G>(ThrowStatement)
    | bf::PR<G>(TryStatement)
    | bf::PR<G>(DebuggerStatement)
    ;

bf::DefineNonTerminal<G, "LetOrConst"> LetOrConst
    = bf::PR<G>(LET)
    | bf::PR<G>(CONST)
    ;

bf::DefineNonTerminal<G, "LexicalBinding"> LexicalBinding
    = bf::PR<G>(Identifier)
    | (Identifier + Initializer)
    | (BindingPattern + Initializer)
    ;

bf::DefineNonTerminal<G, "BindingList"> BindingList
    = bf::PR<G>(LexicalBinding)
    | (BindingList + COMMA + LexicalBinding)
    ;

bf::DefineNonTerminal<G, "LexicalDeclaration"> LexicalDeclaration
    = (LetOrConst + BindingList + SEMI)
    ;

bf::DefineNonTerminal<G, "Declaration"> Declaration
    = bf::PR<G>(LexicalDeclaration)
    | bf::PR<G>(FunctionExpression)
    | bf::PR<G>(ClassExpression)
    | bf::PR<G>(AsyncFunctionExpression)
    | bf::PR<G>(GeneratorExpression)
    | bf::PR<G>(AsyncGeneratorExpression)
    ;

bf::DefineNonTerminal<G, "StatementListItem"> StatementListItem
    = bf::PR<G>(Statement)
    | bf::PR<G>(Declaration)
    ;

bf::DefineNonTerminal<G, "StatementList"> StatementList
    = bf::PR<G>(StatementListItem)
    | (StatementList + StatementListItem)
    ;

bf::DefineNonTerminal<G, "Block"> Block
    = (CURLY_LEFT + CURLY_RIGHT)
    | (CURLY_LEFT + StatementList + CURLY_RIGHT)
    ;

bf::DefineNonTerminal<G, "ScriptBody"> ScriptBody
    = bf::PR<G>(StatementList)
    ;

bf::DefineNonTerminal<G, "Script"> Script
    = bf::PR<G>(ScriptBody)
    ;

bf::DefineNonTerminal<G, "ModuleExportName"> ModuleExportName
    = bf::PR<G>(Identifier)
    | bf::PR<G>(StringLiteral)
    ;

bf::DefineNonTerminal<G, "ImportSpecifier"> ImportSpecifier
    = bf::PR<G>(ModuleExportName)
    | (ModuleExportName + AS + Identifier)
    ;

bf::DefineNonTerminal<G, "ImportsList"> ImportsList
    = bf::PR<G>(ImportSpecifier)
    | (ImportsList + COMMA + ImportSpecifier)
    ;

bf::DefineNonTerminal<G, "NamedImports"> NamedImports
    = (CURLY_LEFT + CURLY_RIGHT)
    | (CURLY_LEFT + ImportsList + CURLY_RIGHT)
    | (CURLY_LEFT + ImportsList + COMMA + CURLY_RIGHT)
    ;

bf::DefineNonTerminal<G, "NameSpaceImport"> NameSpaceImport
    = (STAR + AS + Identifier)
    ;

bf::DefineNonTerminal<G, "ModuleSpecifier"> ModuleSpecifier
    = bf::PR<G>(StringLiteral)
    ;

bf::DefineNonTerminal<G, "FromClause"> FromClause
    = (FROM + ModuleSpecifier)
    ;

bf::DefineNonTerminal<G, "ImportClause"> ImportClause
    = bf::PR<G>(Identifier)
    | bf::PR<G>(NameSpaceImport)
    | bf::PR<G>(NamedImports)
    | (Identifier + COMMA + NameSpaceImport)
    | (Identifier + COMMA + NamedImports)
    ;

bf::DefineNonTerminal<G, "ImportDeclaration"> ImportDeclaration
    = (IMPORT + ImportClause + FromClause + SEMI)
    | (IMPORT + ModuleSpecifier + SEMI)
    ;

bf::DefineNonTerminal<G, "ExportSpecifier"> ExportSpecifier
    = bf::PR<G>(ModuleExportName)
    | (ModuleExportName + AS + ModuleExportName)
    ;

bf::DefineNonTerminal<G, "ExportsList"> ExportsList
    = bf::PR<G>(ExportSpecifier)
    | (ExportsList + COMMA + ExportSpecifier)
    ;

bf::DefineNonTerminal<G, "NamedExports"> NamedExports
    = (CURLY_LEFT + CURLY_RIGHT)
    | (CURLY_LEFT + ExportsList + CURLY_RIGHT)
    | (CURLY_LEFT + ExportsList + COMMA + CURLY_RIGHT)
    ;

bf::DefineNonTerminal<G, "ExportFromClause"> ExportFromClause
    = bf::PR<G>(STAR)
    | (STAR + AS + ModuleExportName)
    | bf::PR<G>(NamedExports)
    ;

bf::DefineNonTerminal<G, "ExportDeclaration"> ExportDeclaration
    = (EXPORT + ExportFromClause + FromClause + SEMI)
    | (EXPORT + NamedExports)
    | (EXPORT + Declaration)
    //| (EXPORT + DEFAULT + FunctionExpression + SEMI)
    //| (EXPORT + DEFAULT + ClassExpression + SEMI)
    //| (EXPORT + DEFAULT + AssignmentExpression + SEMI).Short(FUNCTION).Short(ASYNC).Short(CLASS)
    ;

bf::DefineNonTerminal<G, "ModuleItem"> ModuleItem
    = bf::PR<G>(ImportDeclaration)
    | bf::PR<G>(ExportDeclaration)
    | bf::PR<G>(StatementListItem)
    ;

bf::DefineNonTerminal<G, "ModuleItemList", ast::NodeList> ModuleItemList
    = bf::PR<G>(ModuleItem)<=>[](auto &$)
    {
        $ = ast::NodeList{ .elements = { $(0) }};
    }
    | (ModuleItemList + ModuleItem)<=>[](auto &$)
    {
        //ModuleItemList($[0]).elements.push_back($(1));

        $ = $[0];
    }
    ;

bf::DefineNonTerminal<G, "Module", ast::Module> Module
    = bf::PR<G>(ModuleItemList)<=>[](auto &$)
    {
        $ = ast::Module{ .body = ModuleItemList($[0]) };
    }
    ;

std::expected<bf::SLRParser<GrammarType>, bf::Error> v6::parser::Parser = bf::SLRParser<G>::Build(Module);
