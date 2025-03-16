#include "bbjs/parser/Parser.h"

using namespace bbjs::parser;

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
bf::DefineTerminal<G, R"(from)"> FROM;

bf::DefineTerminal<G, R"(const)"> CONST;
bf::DefineTerminal<G, R"(let)"> LET;

bf::DefineTerminal<G, R"(new)"> NEW;

bf::DefineTerminal<G, R"(for)"> FOR;
bf::DefineTerminal<G, R"(while)"> WHILE;

bf::DefineTerminal<G, R"(function)"> FUNCTION;
bf::DefineTerminal<G, R"(this)"> THIS;
bf::DefineTerminal<G, R"(return)"> RETURN;

bf::DefineTerminal<G, R"(class)"> CLASS;
bf::DefineTerminal<G, R"(extends)"> EXTENDS;
bf::DefineTerminal<G, R"(static)"> STATIC;
bf::DefineTerminal<G, R"(get)"> GET;
bf::DefineTerminal<G, R"(set)"> SET;

bf::DefineTerminal<G, R"(true)"> TRUE;
bf::DefineTerminal<G, R"(false)"> FALSE;

bf::DefineTerminal<G, R"(null)"> KW_NULL;
bf::DefineTerminal<G, R"(undefined)"> UNDEFINED;

// SYMBOLS
bf::DefineTerminal<G, R"(;)"> SEMI;
bf::DefineTerminal<G, R"(:)"> COLON;
bf::DefineTerminal<G, R"(\.)"> DOT(bf::Left);
bf::DefineTerminal<G, R"(\?\.)"> UNCERTAIN_DOT(bf::Left);
bf::DefineTerminal<G, R"(\.{3})"> SPREAD;
bf::DefineTerminal<G, R"(,)"> COMMA;
bf::DefineTerminal<G, R"(#)"> POUND;

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

bf::DefineTerminal<G, R"(\<)"> BINCOMP_LT(bf::Left);
bf::DefineTerminal<G, R"(\>)"> BINCOMP_GT(bf::Left);

// LITERALS
bf::DefineTerminal<G, R"(\d+(\.\d+)?)"> NUMBER;

bf::DefineTerminal<G, R"("[^"]*")"> DOUBLE_STRING;
bf::DefineTerminal<G, R"('[^']*')"> SINGLE_STRING;
bf::DefineTerminal<G, R"(`[^`]*`)"> FORMAT_STRING;

// IDENTIFIERS
bf::DefineTerminal<G, R"([a-zA-Z\d]+)"> IDENTIFIER;

/*
 * THE GRAMMAR
 * Modelled as much as possible after the grammar specification in
 * https://ecma-international.org/wp-content/uploads/ECMA-262_15th_edition_june_2024.pdf
 */
extern bf::DefineNonTerminal<G, "Expression"> Expression;
extern bf::DefineNonTerminal<G, "LeftHandSideExpression"> LeftHandSideExpression;
extern bf::DefineNonTerminal<G, "AssignmentExpression"> AssignmentExpression;
extern bf::DefineNonTerminal<G, "StatementList"> StatementList;

bf::DefineNonTerminal<G, "Identifier"> Identifier
    = bf::PR<G>(IDENTIFIER)
    ;

bf::DefineNonTerminal<G, "PrivateIdentifier"> PrivateIdentifier
    = (POUND + Identifier)
    ;

bf::DefineNonTerminal<G, "Literal"> Literal
    = bf::PR<G>(NUMBER)
    ;

bf::DefineNonTerminal<G, "ArrayLiteral"> ArrayLiteral
    = (SQUARE_LEFT + SQUARE_RIGHT)
    ;

bf::DefineNonTerminal<G, "ObjectLiteral"> ObjectLiteral
    = (CURLY_LEFT + CURLY_RIGHT)
    ;

extern bf::DefineNonTerminal<G, "StringLiteral"> StringLiteral;

extern bf::DefineNonTerminal<G, "NumericLiteral"> NumericLiteral;

bf::DefineNonTerminal<G, "FormalParameter"> FormalParameter
    = bf::PR<G>(Identifier)
    ;

extern bf::DefineNonTerminal<G, "TemplateLiteral"> TemplateLiteral;

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

bf::DefineNonTerminal<G, "FunctionStatementList"> FunctionStatementList
    = bf::PR<G>(StatementList)
    ;

bf::DefineNonTerminal<G, "FunctionBody"> FunctionBody
    = bf::PR<G>(FunctionStatementList)
    ;

bf::DefineNonTerminal<G, "FunctionDeclaration"> FunctionDeclaration
    // Named Function
    = (FUNCTION + Identifier + PAR_LEFT + PAR_RIGHT + CURLY_LEFT + FunctionBody + CURLY_RIGHT)
    | (FUNCTION + Identifier + PAR_LEFT + FormalParameters + PAR_RIGHT + CURLY_LEFT + FunctionBody + CURLY_RIGHT)

    // Anonymous Function
    | (FUNCTION + PAR_LEFT + PAR_RIGHT + CURLY_LEFT + FunctionBody + CURLY_RIGHT)
    | (FUNCTION + PAR_LEFT + FormalParameters + PAR_RIGHT + CURLY_LEFT + FunctionBody + CURLY_RIGHT)
    ;

bf::DefineNonTerminal<G, "FunctionExpression"> FunctionExpression
    = (FUNCTION + Identifier + PAR_LEFT + PAR_RIGHT + CURLY_LEFT + FunctionBody + CURLY_RIGHT)
    | (FUNCTION + Identifier + PAR_LEFT + FormalParameters + PAR_RIGHT + CURLY_LEFT + FunctionBody + CURLY_RIGHT)
    ;

extern bf::DefineNonTerminal<G, "AsyncGeneratorMethod"> AsyncGeneratorMethod;
extern bf::DefineNonTerminal<G, "AsyncGeneratorExpression"> AsyncGeneratorExpression;

extern bf::DefineNonTerminal<G, "AsyncMethod"> AsyncMethod;
extern bf::DefineNonTerminal<G, "AsyncFunctionExpression"> AsyncFunctionExpression;

extern bf::DefineNonTerminal<G, "GeneratorMethod"> GeneratorMethod;
extern bf::DefineNonTerminal<G, "GeneratorExpression"> GeneratorExpression;

bf::DefineNonTerminal<G, "ClassStaticBlockStatementList"> ClassStaticBlockStatementList
    = bf::PR<G>(StatementList)
    ;

bf::DefineNonTerminal<G, "ClassStaticBlockBody"> ClassStaticBlockBody
    = bf::PR<G>(ClassStaticBlockStatementList)
    ;

bf::DefineNonTerminal<G, "ClassStaticBlock"> ClassStaticBlock
    = (STATIC + CURLY_LEFT + CURLY_RIGHT)
    = (STATIC + CURLY_LEFT + ClassStaticBlockBody + CURLY_RIGHT)
    ;

bf::DefineNonTerminal<G, "LiteralPropertyName"> LiteralPropertyName
    = bf::PR<G>(Identifier)
    | bf::PR<G>(StringLiteral)
    | bf::PR<G>(NumericLiteral)
    ;

bf::DefineNonTerminal<G, "ComputedPropertyName"> ComputedPropertyName
    = (SQUARE_LEFT + AssignmentExpression + SQUARE_RIGHT)
    ;

bf::DefineNonTerminal<G, "PropertyName"> PropertyName
    = bf::PR<G>(LiteralPropertyName)
    | bf::PR<G>(ComputedPropertyName)
    ;

bf::DefineNonTerminal<G, "ClassElementName"> ClassElementName
    = bf::PR<G>(PropertyName)
    | bf::PR<G>(PrivateIdentifier)
    ;

bf::DefineNonTerminal<G, "PropertySetParameterList"> PropertySetParameterList
    = bf::PR<G>(FormalParameter)
    ;

bf::DefineNonTerminal<G, "MethodDefinition"> MethodDefinition
    = (ClassElementName + PAR_LEFT + UniqueFormalParameters + PAR_RIGHT + CURLY_LEFT + FunctionBody + CURLY_RIGHT)
    | bf::PR<G>(GeneratorMethod)
    | bf::PR<G>(AsyncMethod)
    | bf::PR<G>(AsyncGeneratorMethod)
    | (GET + ClassElementName + PAR_LEFT + PAR_RIGHT + CURLY_LEFT + FunctionBody + CURLY_RIGHT)
    | (SET + ClassElementName + PAR_LEFT + PropertySetParameterList + PAR_RIGHT + CURLY_LEFT + FunctionBody + CURLY_RIGHT)
    ;

bf::DefineNonTerminal<G, "Initializer"> Initializer
    = (EQUAL + AssignmentExpression)
    ;

bf::DefineNonTerminal<G, "FieldDefinition"> FieldDefinition
    = bf::PR<G>(ClassElementName)
    | (ClassElementName + Initializer)
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

extern bf::DefineNonTerminal<G, "RegularExpressionLiteral"> RegularExpressionLiteral;

extern bf::DefineNonTerminal<G, "BindingPattern"> BindingPattern;

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
    | bf::PR<G>(ArrayLiteral)
    | bf::PR<G>(ObjectLiteral)
    | bf::PR<G>(FunctionExpression)
    | bf::PR<G>(ClassExpression)
    | bf::PR<G>(GeneratorExpression)
    | bf::PR<G>(AsyncFunctionExpression)
    | bf::PR<G>(AsyncGeneratorExpression)
    | bf::PR<G>(RegularExpressionLiteral)
    | bf::PR<G>(CoverParenthesizedExpressionAndArrowParameterList)
    ;

extern bf::DefineNonTerminal<G, "ConditionalExpression"> ConditionalExpression;

extern bf::DefineNonTerminal<G, "YieldExpression"> YieldExpression;

bf::DefineNonTerminal<G, "ArrowParameters"> ArrowParameters
    = bf::PR<G>(Identifier)
    | bf::PR<G>(CoverParenthesizedExpressionAndArrowParameterList)
    ;

bf::DefineNonTerminal<G, "ExpressionBody"> ExpressionBody
    = bf::PR<G>(AssignmentExpression)
    ;

bf::DefineNonTerminal<G, "ConciseBody"> ConciseBody
    = bf::PR<G>(ExpressionBody)
    | (CURLY_LEFT + FunctionBody + CURLY_RIGHT)
    ;

bf::DefineNonTerminal<G, "ArrowFunction"> ArrowFunction
    = (ArrowParameters + ARROW + ConciseBody)
    ;

extern bf::DefineNonTerminal<G, "AsyncArrowFunction"> AsyncArrowFunction;

extern bf::DefineNonTerminal<G, "SuperProperty"> SuperProperty;

extern bf::DefineNonTerminal<G, "MetaProperty"> MetaProperty;

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

extern bf::DefineNonTerminal<G, "CoverCallExpressionAndAsyncArrowHead"> CoverCallExpressionAndAsyncArrowHead;

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

bf::DefineNonTerminal<G, "AssignmentExpression"> AssignmentExpression
    = bf::PR<G>(ConditionalExpression)
    | bf::PR<G>(YieldExpression)
    | bf::PR<G>(ArrowFunction)
    | bf::PR<G>(AsyncArrowFunction)
    | (LeftHandSideExpression + EQUAL + AssignmentExpression)
    ;

bf::DefineNonTerminal<G, "Expression"> Expression
    = bf::PR<G>(AssignmentExpression)
    | (Expression + COMMA + AssignmentExpression)
    ;

extern bf::DefineNonTerminal<G, "BlockStatement"> BlockStatement;
extern bf::DefineNonTerminal<G, "VariableStatement"> VariableStatement;
extern bf::DefineNonTerminal<G, "EmptyStatement"> EmptyStatement;
extern bf::DefineNonTerminal<G, "ExpressionStatement"> ExpressionStatement;
extern bf::DefineNonTerminal<G, "IfStatement"> IfStatement;
extern bf::DefineNonTerminal<G, "BreakableStatement"> BreakableStatement;
extern bf::DefineNonTerminal<G, "ContinueStatement"> ContinueStatement;
extern bf::DefineNonTerminal<G, "BreakStatement"> BreakStatement;
extern bf::DefineNonTerminal<G, "ReturnStatement"> ReturnStatement;
extern bf::DefineNonTerminal<G, "WithStatement"> WithStatement;
extern bf::DefineNonTerminal<G, "LabelledStatement"> LabelledStatement;
extern bf::DefineNonTerminal<G, "ThrowStatement"> ThrowStatement;
extern bf::DefineNonTerminal<G, "TryStatement"> TryStatement;
extern bf::DefineNonTerminal<G, "DebuggerStatement"> DebuggerStatement;

bf::DefineNonTerminal<G, "Statement"> Statement
    = bf::PR<G>(BlockStatement)
    | bf::PR<G>(VariableStatement)
    | bf::PR<G>(EmptyStatement)
    | bf::PR<G>(ExpressionStatement)
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

extern bf::DefineNonTerminal<G, "GeneratorDeclaration"> GeneratorDeclaration;
extern bf::DefineNonTerminal<G, "AsyncFunctionDeclaration"> AsyncFunctionDeclaration;
extern bf::DefineNonTerminal<G, "AsyncGeneratorDeclaration"> AsyncGeneratorDeclaration;

bf::DefineNonTerminal<G, "HoistableDeclaration"> HoistableDeclaration
    = bf::PR<G>(FunctionDeclaration)
    | bf::PR<G>(GeneratorDeclaration)
    | bf::PR<G>(AsyncFunctionDeclaration)
    | bf::PR<G>(AsyncGeneratorDeclaration)
    ;

extern bf::DefineNonTerminal<G, "ClassDeclaration"> ClassDeclaration;
extern bf::DefineNonTerminal<G, "LexicalDeclaration"> LexicalDeclaration;

bf::DefineNonTerminal<G, "Declaration"> Declaration
    = bf::PR<G>(HoistableDeclaration)
    | bf::PR<G>(ClassDeclaration)
    | bf::PR<G>(LexicalDeclaration)
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
    = (CURLY_LEFT + StatementList + CURLY_RIGHT)
    ;

bf::DefineNonTerminal<G, "BlockStatement"> BlockStatement
    = bf::PR<G>(Block)
    ;

bf::DefineNonTerminal<G, "ScriptBody"> ScriptBody
    = bf::PR<G>(StatementList)
    ;

bf::DefineNonTerminal<G, "Script"> bbjs::parser::Script
    = bf::PR<G>(ScriptBody)
    ;
