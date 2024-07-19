//#include <iostream>
//#include <vector>
//#include <string>
//#include <cctype>
//#include <stdexcept>
//#include <memory>
//#include <cstdarg>
//#include "symbol.h"
//// Token Types
//enum TokenType {
//    TOKEN_INTEGER,
//    TOKEN_PLUS,
//    TOKEN_MULTIPLY,
//    TOKEN_LPAREN,
//    TOKEN_RPAREN,
//    TOKEN_IF,
//    TOKEN_THEN,
//    TOKEN_ELSE,
//    TOKEN_FI,
//    TOKEN_END
//};

//// Token Structure
//struct Token {
//    TokenType type;
//    int value;
//    Token(TokenType type, int value = 0) : type(type), value(value) {}
//};

//// AST Types and Structures
//typedef enum {
//    ast_var_decl,
//    ast_const_decl,
//    ast_routine_decl,
//    ast_assign,
//    ast_if,
//    ast_while,
//    ast_for,
//    ast_read,
//    ast_write,
//    ast_call,
//    ast_block,
//    ast_return,
//    ast_var,
//    ast_integer,
//    ast_float,
//    ast_string,
//    ast_boolean,
//    ast_times,
//    ast_divide,
//    ast_plus,
//    ast_minus,
//    ast_eq,
//    ast_neq,
//    ast_lt,
//    ast_le,
//    ast_gt,
//    ast_ge,
//    ast_and,
//    ast_or,
//    ast_cand,
//    ast_cor,
//    ast_not,
//    ast_uminus,
//    ast_itof,
//    ast_eof
//} AST_type;

//struct symbol_table_entry {};
//struct ste_list {};
//struct ast_list {};

//struct AST {
//    AST_type type;
//    union {
//        struct { symbol_table_entry* name; j_type type; } a_var_decl;
//        struct { symbol_table_entry* name; int value; } a_const_decl;
//        struct { symbol_table_entry* name; ste_list* formals; j_type result_type; AST* body; } a_routine_decl;
//        struct { symbol_table_entry* lhs; AST* rhs; } a_assign;
//        struct { AST* predicate; AST* conseq; AST* altern; } a_if;
//        struct { AST* predicate; AST* body; } a_while;
//        struct { symbol_table_entry* var; AST* lower_bound; AST* upper_bound; AST* body; } a_for;
//        struct { symbol_table_entry* var; } a_read;
//        struct { symbol_table_entry* var; } a_write;
//        struct { symbol_table_entry* callee; ast_list* arg_list; } a_call;
//        struct { ste_list* vars; ast_list* stmts; } a_block;
//        struct { AST* expr; } a_return;
//        struct { symbol_table_entry* var; } a_var;
//        struct { int value; } a_integer;
//        struct { float value; } a_float;
//        struct { char* string; } a_string;
//        struct { int value; } a_boolean;
//        struct { AST* larg; AST* rarg; } a_binary_op;
//        struct { AST* arg; } a_unary_op;
//        struct { AST* arg; } a_itof;
//    } f;
//};

//// Function to report fatal errors
//void fatal_error(const char* msg) {
//    std::cerr << msg << std::endl;
//    std::exit(1);
//}

//// Function to create AST nodes
//AST* make_ast_node(int type, ...) {
//    AST* n = new AST;
//    va_list ap;

//    if (n == nullptr) {
//        fatal_error("malloc failed in make_ast_node\n");
//    }

//    va_start(ap, type);
//    n->type = static_cast<AST_type>(type);

//    switch (n->type) {
//        case ast_var_decl:
//            n->f.a_var_decl.name = va_arg(ap, symbol_table_entry*);
//            n->f.a_var_decl.type = va_arg(ap, j_type);
//            break;
//        case ast_const_decl:
//            n->f.a_const_decl.name = va_arg(ap, symbol_table_entry*);
//            n->f.a_const_decl.value = va_arg(ap, int);
//            break;
//        case ast_routine_decl:
//            n->f.a_routine_decl.name = va_arg(ap, symbol_table_entry*);
//            n->f.a_routine_decl.formals = va_arg(ap, ste_list*);
//            n->f.a_routine_decl.result_type = va_arg(ap, j_type);
//            n->f.a_routine_decl.body = va_arg(ap, AST*);
//            break;
//        case ast_assign:
//            n->f.a_assign.lhs = va_arg(ap, symbol_table_entry*);
//            n->f.a_assign.rhs = va_arg(ap, AST*);
//            break;
//        case ast_if:
//            n->f.a_if.predicate = va_arg(ap, AST*);
//            n->f.a_if.conseq = va_arg(ap, AST*);
//            n->f.a_if.altern = va_arg(ap, AST*);
//            break;
//        case ast_while:
//            n->f.a_while.predicate = va_arg(ap, AST*);
//            n->f.a_while.body = va_arg(ap, AST*);
//            break;
//        case ast_for:
//            n->f.a_for.var = va_arg(ap, symbol_table_entry*);
//            n->f.a_for.lower_bound = va_arg(ap, AST*);
//            n->f.a_for.upper_bound = va_arg(ap, AST*);
//            n->f.a_for.body = va_arg(ap, AST*);
//            break;
//        case ast_read:
//            n->f.a_read.var = va_arg(ap, symbol_table_entry*);
//            break;
//        case ast_write:
//            n->f.a_write.var = va_arg(ap, symbol_table_entry*);
//            break;
//        case ast_call:
//            n->f.a_call.callee = va_arg(ap, symbol_table_entry*);
//            n->f.a_call.arg_list = va_arg(ap, ast_list*);
//            break;
//        case ast_block:
//            n->f.a_block.vars = va_arg(ap, ste_list*);
//            n->f.a_block.stmts = va_arg(ap, ast_list*);
//            break;
//        case ast_return:
//            n->f.a_return.expr = va_arg(ap, AST*);
//            break;
//        case ast_var:
//            n->f.a_var.var = va_arg(ap, symbol_table_entry*);
//            break;
//        case ast_integer:
//            n->f.a_integer.value = va_arg(ap, int);
//            break;
//        case ast_float:
//            n->f.a_float.value = static_cast<float>(va_arg(ap, double)); // va_arg promotes float to double
//            break;
//        case ast_string:
//            n->f.a_string.string = va_arg(ap, char*);
//            break;
//        case ast_boolean:
//            n->f.a_boolean.value = va_arg(ap, int);
//            break;
//        case ast_times:
//        case ast_divide:
//        case ast_plus:
//        case ast_minus:
//        case ast_eq:
//        case ast_neq:
//        case ast_lt:
//        case ast_le:
//        case ast_gt:
//        case ast_ge:
//        case ast_and:
//        case ast_or:
//        case ast_cand:
//        case ast_cor:
//            n->f.a_binary_op.larg = va_arg(ap, AST*);
//            n->f.a_binary_op.rarg = va_arg(ap, AST*);
//            break;
//        case ast_not:
//        case ast_uminus:
//            n->f.a_unary_op.arg = va_arg(ap, AST*);
//            break;
//        case ast_itof:
//            n->f.a_itof.arg = va_arg(ap, AST*);
//            break;
//        case ast_eof:
//            break;
//        default:
//            fatal_error("Unknown type of AST node in make_ast_node\n");
//    }

//    va_end(ap);
//    return n;
//}


//// Parser Structure
//struct Parser {
//    std::vector<Token> tokens;
//    size_t position;

//    Parser(const std::vector<Token>& tokens) : tokens(tokens), position(0) {}

//    Token current_token() {
//        if (position < tokens.size()) {
//            return tokens[position];
//        } else {
//            std::cerr << "No more tokens to parse" << std::endl;
//            return {TOKEN_END, 0};
//        }
//    }

//    Token eat_token(TokenType type) {
//        Token token = current_token();
//        printf("TOK %d",token.value);
//        if (token.type != type) {
//            std::cerr << "Expected token type: " << type << ", but got token type: " << token.type << std::endl;
//            std::exit(1); // Exit or throw an exception here depending on your error handling strategy
//        }
//        ++position;
//        std::cout << "Consumed token type: " << type << std::endl; // Debug output
//        return token;
//    }

//    std::unique_ptr<AST> parse() {
//        return parse_statement();
//    }

//private:
//    std::unique_ptr<AST> parse_statement() {
//        Token token = current_token();
//        if (token.type == TOKEN_IF) {
//            return parse_if_statement();
//        } else {
//            return parse_expression();
//        }
//    }

//    std::unique_ptr<AST> parse_if_statement() {

//        eat_token(TOKEN_IF);

//        auto predicate = parse_expression();

//        eat_token(TOKEN_THEN);

//        auto conseq = parse_statement();

//        if (current_token().type == TOKEN_ELSE) {

//            eat_token(TOKEN_ELSE);
//            auto altern = parse_statement();
//            eat_token(TOKEN_FI);
//            return std::unique_ptr<AST>(make_ast_node(ast_if, predicate.release(), conseq.release(), altern.release()));
//        } else {
//            printf("Hello  \n");

//            eat_token(TOKEN_FI);
//            return std::unique_ptr<AST>(make_ast_node(ast_if, predicate.release(), conseq.release(), nullptr));
//        }
//    }

//    std::unique_ptr<AST> parse_expression() {

//        return parse_term();
//    }

//    std::unique_ptr<AST> parse_term() {

//        auto node = parse_factor();

//        while (current_token().type == TOKEN_PLUS) {
//            eat_token(TOKEN_PLUS);
//            node = std::unique_ptr<AST>(make_ast_node(ast_plus, node.release(), parse_factor().release()));
//        }

//        return node;
//    }

//    std::unique_ptr<AST> parse_factor() {
//        Token token = current_token();

//        if (token.type == TOKEN_INTEGER) {
//            eat_token(TOKEN_INTEGER);
//            return std::unique_ptr<AST>(make_ast_node(ast_integer, token.value));
//        } else if (token.type == TOKEN_LPAREN) {
//            eat_token(TOKEN_LPAREN);
//            auto node = parse_expression();
//            eat_token(TOKEN_RPAREN);
//            return node;
//        } else {
//            throw std::runtime_error("Unexpected token 3");
//        }
//    }
//};

//int main() {
//    // Example tokens for testing if-else parsing
//    std::vector<Token> tokens = {
//           {TOKEN_IF},
//           {TOKEN_INTEGER, 3},
//           {TOKEN_PLUS},
//           {TOKEN_INTEGER, 4},
//           {TOKEN_THEN},
//           {TOKEN_INTEGER, 7},
//            {TOKEN_ELSE},
//            {TOKEN_INTEGER, 5},

//           {TOKEN_FI},
//           {TOKEN_END}
//       };


//    Parser parser(tokens);
//    try {
//        auto ast = parser.parse();
//        // Print or process the AST as needed
//        std::cout << "AST parsed successfully!" << std::endl;
//    } catch (const std::exception& e) {
//        std::cerr << "Error parsing: " << e.what() << std::endl;
//    }

//    return 0;
//}
