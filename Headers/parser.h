#ifndef PARSER
#define PARSER
#include <iostream>
#include "symbol.h"
#include "scanner.h"
#include <fstream>
#include <vector>
#include <string>
#include <cctype>
#include <QString>
#include <stdexcept>
#include <memory>
#include <cstdarg>
enum TokenType {
    TOK_END,
    TOK_INTEGER,
    TOK_ID,
    TOK_VAR,
    TOK_CONST,
    TOK_INT,
    TOK_PLUS,
    TOK_MINUS,
    TOK_TIMES,
    TOK_DIVIDE,
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_ASSIGN,
    TOK_SEMICOLON,
    TOK_ASSIGN_COLON,
    TOK_COLON,
    TOK_GE,
    TOK_GT,
    TOK_LT,
    TOK_LE,
    TOK_AND,
    TOK_OR,
    TOK_EQ,
    TOK_NEQ,
    TOK_NOT,
    TOK_STRING,
    TOK_FLOAT,
    TOK_IF,
    TOK_FI,
    TOK_THEN,
    TOK_ELSE,
    TOK_WHILE,
    TOK_DO,
    TOK_OD,
    TOK_FOR,
    TOK_TO,
    TOK_Begin,
    TOK_Endkw,
    TOK_ERROR
};
typedef enum {
    ast_var_decl,
    ast_const_decl,
    ast_routine_decl,
    ast_assign,
    ast_if,
    ast_while,
    ast_for,
    ast_read,
    ast_write,
    ast_call,
    ast_block,
    ast_return,
    ast_var,
    ast_integer,
    ast_float,
    ast_string,
    ast_boolean,
    ast_times,
    ast_divide,
    ast_plus,
    ast_minus,
    ast_eq,
    ast_neq,
    ast_lt,
    ast_le,
    ast_gt,
    ast_ge,
    ast_and,
    ast_or,
    ast_cand,
    ast_cor,
    ast_not,
    ast_uminus,
    ast_itof,
    ast_eof
} AST_type;

struct Token {
    TokenType type;
    union {
        int value;
        float value_f;
    };
    std::string text;
};

struct ste_list {};
typedef struct ast_list_cell
{
 struct AST *head; // pointer to  a tree
 struct ast_list_cell *tail;  // this is the next pointer that you use typically in the list
} ast_list;


struct AST {
    AST_type type;
    union {
        struct { SymbolTableEntry* name; j_type type; } a_var_decl;
        struct { SymbolTableEntry* name; int value; } a_const_decl;
        struct { SymbolTableEntry* name; ste_list* formals; j_type result_type; AST* body; } a_routine_decl;
        struct { SymbolTableEntry* lhs; AST* rhs; } a_assign;
        struct { AST* predicate; AST* conseq; AST* altern; } a_if;
        struct { AST* predicate; AST* body; } a_while;
        struct { SymbolTableEntry* var; AST* lower_bound; AST* upper_bound; AST* body; } a_for;
        struct { SymbolTableEntry* var; } a_read;
        struct { SymbolTableEntry* var; } a_write;
        struct { SymbolTableEntry* callee; ast_list* arg_list; } a_call;
        struct { ste_list* vars; ast_list* stmts; } a_block;
        struct { AST* expr; } a_return;
        struct { SymbolTableEntry* var; } a_var;
        struct { int value; } a_integer;
        struct { float value; } a_float;
        struct { char* string; } a_string;
        struct { int value; } a_boolean;
        struct { AST* larg; AST* rarg; } a_binary_op;
        struct { AST* arg; } a_unary_op;
        struct { AST* arg; } a_itof;
    } f;
};

class Parser {
public:
    Parser(FileDescriptor* fd) : scanner(fd) {
        get_token();
    }

    void parse();
    AST* parse_expression(int flage);
    SymbolTable st;

private:
    Token token;
    SCANNER scanner;
    void wr_error(const QString& msg);
    AST* parse_declaration(int flage);
    AST* parse_binary(int flage);         // For primary expressions like integers, identifiers, parenthesis
    AST* parse_unary(int flage);           // For unary operations
    AST* parse_multiplicative(int flage);  // For * and /
    AST* parse_additive(int flage);        // For + and -
    AST* parse_relational(int flage);      // For relational operations
    AST* parse_logical(int flage);         // For logical conjunctions
    void expect(TokenType expected);  // For ensuring expected token types
    AST* parse_if_statement();
    AST* parse_statement();
    AST* parse_while_loop();
    AST* parse_for_loop();
    AST* parse_block();
    void get_token();  // Method to get the next token
   void write_error(char* msg);
};

#endif // PARSER

