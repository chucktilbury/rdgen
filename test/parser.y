%debug
%defines
%locations

%{

#include <stdio.h>
#include <stdint.h>

#include "scanner.h"

#define TRACE(fmt, ...) do { \
            printf((fmt), ## __VA_ARGS__); \
            fputc('\n', stdout); \
        } while(0)

%}

%union {
    char* str;
    char* symbol;
    double fnum;
    unsigned unum;
    int inum;
    int type;

};

%token BREAK CONTINUE CONST NAMESPACE LIST DICT
%token DO ELSE IF RETURN IMPORT CLASS TRY EXCEPT RAISE
%token TRUE FALSE YIELD WHILE PUBLIC PRIVATE PROTECTED
%token EQU NEQU LORE GORE OR AND NOT AS
%token ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN
%token UNSIGNED INTEGER FLOAT NOTHING STRING BOOLEAN
%token PRINT TRACE TYPE

%token<symbol> SYMBOL
%token<fnum> FNUM
%token<inum> INUM
%token<unum> UNUM
%token<str> STRG

%define parse.error verbose
%locations

%left OR
%left AND
%left EQU NEQU
%left LORE GORE '<' '>'
%left '+' '-'
%left '*' '/' '%'
%right '^'
%left NEGATE

%glr-parser

%%

module
    : module_item {}
    | module module_item {}
    ;

module_item
    : namespace_definition {}
    | import_definition {}
    | class_definition {}
    | scope_operator {}
    ;

    /*
     * Namespace definition
     */
namespace_definition
    : NAMESPACE SYMBOL '{' module '}' {}
    ;

import_definition
    : IMPORT compound_name {}
    | IMPORT compound_name AS SYMBOL {}
    ;

    /*
     * Variable declaration is used to define a var name in a class and
     * also in a function. Variables that are defined in a function can
     * have an assignment as well.
     */
name_declaration
    : type_name SYMBOL {}
    ;

variable_declaration
    : name_declaration {}
    | CONST name_declaration {}
    ;

variable_declaration_list
    : variable_declaration {}
    | variable_declaration_list ',' variable_declaration {}
    ;

    /*
     * Misc definitions
     */
type_name
    : UNSIGNED {}
    | INTEGER {}
    | FLOAT {}
    | STRING {}
    | BOOLEAN {}
    | DICT {}
    | LIST {}
    | compound_name {}
    ;

scope_operator
    : PUBLIC {}
    | PRIVATE {}
    | PROTECTED {}
    ;

literal_value
    : STRG {}
    | INUM {}
    | UNUM {}
    | FNUM {}
    ;

bool_value
    : TRUE {}
    | FALSE {}
    ;

    /*
     * Name types
     */
array_reference
    : SYMBOL '[' expression_element ']' {}
    ;

function_reference
    : SYMBOL '(' ')' {}
    | SYMBOL '(' expression_list ')' {}
    ;

    /*
     * Compound name names a class or a variable without an array
     * reference or a function reference.
     */
compound_reference_element
    : SYMBOL {}
    | array_reference {}
    | function_reference {}
    ;

compound_reference
    : compound_reference_element {}
    | compound_reference '.' compound_reference_element {}
    ;

compound_name
    : SYMBOL {}
    | compound_name '.' SYMBOL {}
    ;

    /*
     * Expressions
     */
expression_element
    : compound_reference {}
    | literal_value {}
    | '(' expression ')' {}
    ;

    /*expression
    : expression_element {}
    | expression OR expression {}
    | expression AND expression {}
    | expression EQU expression {}
    | expression NEQU expression {}
    | expression LORE expression {}
    | expression GORE expression {}
    | expression '<' expression {}
    | expression '>' expression {}
    | expression '+' expression {}
    | expression '-' expression {}
    | expression '*' expression {}
    | expression '/' expression {}
    | expression '%' expression {}
    | expression '^' expression {}
    | '-' expression %prec NEGATE {}
    | '+' expression %prec NEGATE {}
    | NOT expression %prec NEGATE {}
    | '(' expression ')' {}
    ;*/
expression
    : expr_or {}
    ;

expr_or
    : expr_and {}
    | expr_or OR expr_and {}
    ;

expr_and
    : expr_equality {}
    | expr_and AND expr_equality {}
    ;

expr_equality
    : expr_compare {}
    | expr_equality EQU expr_compare {}
    | expr_equality NEQU expr_compare {}
    ;

expr_compare
    : expr_term {}
    | expr_compare LORE expr_term {}
    | expr_compare GORE expr_term {}
    | expr_compare '<' expr_term {}
    | expr_compare '>' expr_term {}
    ;

expr_term
    : expr_factor {}
    | expr_term '+' expr_factor {}
    | expr_term '-' expr_factor {}
    ;

expr_factor
    : expr_unary {}
    | expr_factor '*' expr_unary {}
    | expr_factor '/' expr_unary {}
    | expr_factor '%' expr_unary {}
    ;

expr_unary
    : expression_element {}
    | '-' expr_unary {}
    | '+' expr_unary {}
    | NOT expr_unary {}
    ;

expression_list
    : expression {}
    | expression_list ',' expression {}
    ;

    /*
     * class definition elements.
     */
class_content
    : '{' class_content_list '}'
    | '{' '}'
    ;

class_content_list
    : class_content_element {}
    | class_content_list class_content_element {}
    ;

class_content_element
    : scope_operator {}
    | variable_declaration {}
    | function_definition {}
    ;

class_definition
    : CLASS SYMBOL class_content {}
    | CLASS SYMBOL '(' ')' class_content {}
    | CLASS SYMBOL '(' compound_name ')' class_content {}
    ;

    /*
     * Function definition
     */
function_definition
    : name_declaration '(' variable_declaration_list ')' function_body {}
    | name_declaration '(' ')' function_body {}
    ;

function_body
    : '{' function_body_list '}' {}
    | '{' '}' {}
    ;

function_body_list
    : function_body_element {}
    | function_body_list function_body_element {}
    ;

function_body_element
    : function_body {}
    /*
        Variable_declaration and compound_reference produce a
        reduce/reduce conflict, but the parser still appears to
        work.
     */
    | variable_declaration {}
    | variable_declaration '=' op_assignment_rhs {}
    | compound_reference {}
    | compound_reference '=' op_assignment_rhs {}
    | compound_reference operator_assignment {}
    | if_clause {}
    | try_clause {}
    | while_clause {}
    | do_clause {}
    | RAISE '(' compound_name ')' {}
    | PRINT single_expression {}
    | RETURN single_expression {}
    | TYPE single_expression {}
    | YIELD '(' expression ')' {}
    | BREAK {}
    | CONTINUE {}
    | TRACE {}
    ;

single_expression
    : %empty {}
    | '(' ')' {}
    | '(' expression ')' {}
    ;

assignment_rhs
    : expression {}
    | array_init {}
    | dict_init {}
    | bool_value {}
    ;

op_assignment_rhs
    : assignment_rhs {}
    | type_name '(' assignment_rhs ')' {}
    ;

operator_assignment
    : ADD_ASSIGN expression {}
    | SUB_ASSIGN expression {}
    | MUL_ASSIGN expression {}
    | DIV_ASSIGN expression {}
    | MOD_ASSIGN expression {}
    ;

array_init
    : '[' array_init_list ']' {}
    ;

array_init_list
    : array_init_element {}
    | array_init_list ',' array_init_element {}
    ;

array_init_element
    : expression {}
    | array_init {}
    | dict_init {}
    ;

dict_init
    : '[' dict_init_list ']' {}
    ;

dict_init_list
    : dict_init_element {}
    | dict_init_list ',' dict_init_element {}
    ;

dict_init_element
    : STRG ':' expression {}
    | STRG ':' array_init {}
    | STRG ':' dict_init {}
    ;

test_expression
    : expression {}
    | bool_value {}
    ;

if_clause
    : IF '(' test_expression ')' function_body else_clause {}
    ;

else_clause
    : %empty {}
    | else_clause_list {}
    | else_clause_list else_clause_final {}
    | else_clause_final {}
    ;

else_clause_main
    : ELSE '(' test_expression ')' function_body {}
    ;

else_clause_list
    : else_clause_main {}
    | else_clause_list else_clause_main {}
    ;

else_clause_final
    : ELSE '(' ')' function_body {}
    | ELSE function_body {}
    ;

try_clause
    : TRY function_body except_clause {}
    ;

except_clause
    : except_clause_list {}
    | except_clause_list except_clause_final {}
    | except_clause_final {}
    ;

except_clause_main
    : EXCEPT '(' compound_reference ')' function_body {}
    ;

except_clause_list
    : except_clause_main {}
    | except_clause_list except_clause_main {}
    ;

except_clause_final
    : EXCEPT '(' ')' function_body {}
    | EXCEPT function_body {}
    ;

while_clause
    : WHILE function_body {}
    | WHILE '(' ')' function_body {}
    | WHILE '(' test_expression ')' function_body {}
    ;

do_clause
    : DO function_body WHILE '(' ')' {}
    | DO function_body WHILE '(' test_expression ')' {}
    ;


%%

extern int errors;
void yyerror(const char* s) {

    fprintf(stderr, "%s:%d:%d %s\n",
            get_file_name(), get_line_no(), get_col_no(), s);
    errors++;
}

const char* tokenToStr(int tok) {

    return yysymbol_name(YYTRANSLATE(tok));
}
