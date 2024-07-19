#include "parser.h"
using namespace std;
// Function to report fatal errors
void fatal_error(const char* msg) {
//    std::cerr << msg << std::endl;
//    std::exit(1);
   //scanner.Fd->ReportError(msg.toLocal8Bit().data());;
}

// Function to create AST nodes
AST* make_ast_node(int type, ...) {

    AST* n = new AST;
    va_list ap;

    if (n == nullptr) {
        fatal_error("malloc failed in make_ast_node\n");
    }

    va_start(ap, type);
    n->type = static_cast<AST_type>(type);

    switch (n->type) {
        case ast_var_decl: {
        n->f.a_var_decl.name = va_arg(ap, SymbolTableEntry*);
        // Assuming j_type is a pointer or needs allocation:
//        n->f.a_var_decl.type =  (va_arg(ap, j_type));
            break;
    }
        case ast_const_decl:
            n->f.a_const_decl.name = va_arg(ap, SymbolTableEntry*);
            n->f.a_const_decl.value = va_arg(ap, int);
            break;
        case ast_routine_decl:
            n->f.a_routine_decl.name = va_arg(ap, SymbolTableEntry*);
            n->f.a_routine_decl.formals = va_arg(ap, ste_list*);
            n->f.a_routine_decl.result_type = va_arg(ap, j_type);
            n->f.a_routine_decl.body = va_arg(ap, AST*);
            break;
        case ast_assign:
            n->f.a_assign.lhs = va_arg(ap, SymbolTableEntry*);
            n->f.a_assign.rhs = va_arg(ap, AST*);

            break;
        case ast_if:
            n->f.a_if.predicate = va_arg(ap, AST*);
            n->f.a_if.conseq = va_arg(ap, AST*);
            n->f.a_if.altern = va_arg(ap, AST*);
            break;
        case ast_while:
            n->f.a_while.predicate = va_arg(ap, AST*);
            n->f.a_while.body = va_arg(ap, AST*);
            break;
        case ast_for:
            n->f.a_for.var = va_arg(ap, SymbolTableEntry*);
            n->f.a_for.lower_bound = va_arg(ap, AST*);
            n->f.a_for.upper_bound = va_arg(ap, AST*);
            n->f.a_for.body = va_arg(ap, AST*);
        break;
        case ast_read:
            n->f.a_read.var = va_arg(ap, SymbolTableEntry*);
            break;
        case ast_write:
            n->f.a_write.var = va_arg(ap, SymbolTableEntry*);
            break;
        case ast_call:
            n->f.a_call.callee = va_arg(ap, SymbolTableEntry*);
            n->f.a_call.arg_list = va_arg(ap, ast_list*);
            break;
        case ast_block:
//            n->f.a_block.vars = va_arg(ap, ste_list*);
//            n->f.a_block.stmts = va_arg(ap, ast_list*);

           n->type = ast_block;
           n->f.a_block.stmts =va_arg(ap, ast_list*);

            break;
        case ast_return:
            n->f.a_return.expr = va_arg(ap, AST*);
            break;
        case ast_var: {
            n->f.a_var.var = va_arg(ap, SymbolTableEntry*);
            break;
    }
        case ast_integer:
            n->f.a_integer.value = va_arg(ap, int);
            break;
        case ast_float:
            n->f.a_float.value = static_cast<float>(va_arg(ap, double)); // va_arg promotes float to double
            break;
        case ast_string:
            n->f.a_string.string = va_arg(ap, char*);
            break;
        case ast_boolean:
            n->f.a_boolean.value = va_arg(ap, int);
            break;
        case ast_times:
        case ast_divide:
        case ast_plus:
        case ast_minus:
        case ast_eq:
        case ast_neq:
        case ast_lt:
        case ast_le:
        case ast_gt:
        case ast_ge:
        case ast_and:
        case ast_or:
        case ast_cand:
        case ast_cor:
            n->f.a_binary_op.larg = va_arg(ap, AST*);
            n->f.a_binary_op.rarg = va_arg(ap, AST*);
            break;
        case ast_not:
        case ast_uminus:
            n->f.a_unary_op.arg = va_arg(ap, AST*);
            break;
        case ast_itof:
            n->f.a_itof.arg = va_arg(ap, AST*);
            break;
        case ast_eof:
            break;
        default:
            fatal_error("Unknown type of AST node in make_ast_node\n");
    }

    va_end(ap);
    return n;
}
void printAST(AST* node, int depth) {
    if (node == nullptr) return;

    for (int i = 0; i < depth; ++i) {
        printf("  ");
    }

    switch (node->type) {
        case ast_integer:
            printf("%d\n", node->f.a_integer.value);
            break;
        case ast_plus:
            printf("+\n");
            break;
        case ast_minus:
            printf("-\n");
            break;
        case ast_times:
            printf("*\n");
            break;
        case ast_divide:
            printf("/\n");
            break;
        case ast_eq:
            printf("==\n");
            break;
        case ast_neq:
            printf("!=\n");
            break;
        case ast_lt:
            printf("<\n");
            break;
        case ast_le:
            printf("<=\n");
            break;
        case ast_gt:
            printf(">\n");
            break;
        case ast_ge:
            printf(">=\n");
            break;
        case ast_and:
            printf("&&\n");
            break;
        case ast_or:
            printf("||\n");
            break;
        case ast_not:
            printf("!\n");
            break;
        case ast_var:
        cout<<node->f.a_var.var->name<<endl;
           // printf("%s\n", node->f.a_var.var->name);
            break;
        case ast_var_decl:
            printf("var %s: %d\n", node->f.a_var_decl.name, node->f.a_var_decl.type);
            break;
        case ast_const_decl:
            printf("const %s = %d\n", node->f.a_const_decl.name, node->f.a_const_decl.value);
            break;
        case ast_if:
            printf("if\n");
            break;
        case ast_assign:
            printf(":=\n");
            break;
    case ast_while:
        printf("while\n");
        break;
    case ast_for:
        printf("for\n");
        break;
        case ast_block:
            printf("block\n");
            break;
        // Add more cases for other AST node types...
        default:
            printf("Unknown node type\n");
            break;
    }

    switch (node->type) {
        case ast_plus:
        case ast_minus:
        case ast_times:
        case ast_divide:
        case ast_eq:
        case ast_neq:
        case ast_lt:
        case ast_le:
        case ast_gt:
        case ast_ge:
        case ast_and:
        case ast_or:
            printAST(node->f.a_binary_op.larg, depth + 1);
            printAST(node->f.a_binary_op.rarg, depth + 1);
            break;
        case ast_not:
        case ast_uminus:
            printAST(node->f.a_unary_op.arg, depth + 1);
            break;
        case ast_if:
            printAST(node->f.a_if.predicate, depth + 1);
            printAST(node->f.a_if.conseq, depth + 1);
            printAST(node->f.a_if.altern, depth + 1);

    case ast_while:
        printAST(node->f.a_while.predicate, depth + 1);
        printAST(node->f.a_while.body, depth + 1);
        break;

    case ast_for:
        printAST(node->f.a_for.lower_bound, depth + 1);
        printAST(node->f.a_for.upper_bound, depth + 1);
        printAST(node->f.a_for.body, depth + 1);

        break;

        case ast_assign:
//            printAST(node->f.a_assign.lhs->, depth + 1);
            printAST(node->f.a_assign.rhs, depth + 1);
            break;
      /*  case ast_block:
            // Assuming vars and stmts are lists, iterate through them
            for (auto var : *(node->f.a_block.vars)) {
                printAST(var, depth + 1);
            }
            for (auto stmt : *(node->f.a_block.stmts)) {
                printAST(stmt, depth + 1);
            }
            break;*/
        // Add more cases for other AST node types with child nodes...
        default:
            break;
    }
}



void Parser::get_token() {

    TOKEN *scannedToken = scanner.Scan();
    switch (scannedToken->type) {
        case lx_eof:
            token.type = TOK_END;
            break;
        case lx_integer:

            token.type = TOK_INTEGER;
            token.value = scannedToken->value;

            break;
    case kw_begin:
        token.type = TOK_Begin;
        break;
    case kw_end:
        token.type = TOK_Endkw;
        break;

        case lx_identifier:
            token.type = TOK_ID;
            token.text = scannedToken->str_ptr;
            break;
        case kw_var:
            token.type = TOK_VAR;
            break;
        case kw_constant:
            token.type = TOK_CONST;
            break;
        case kw_integer:
            token.type = TOK_INT;
            break;
        case lx_plus:
            token.type = TOK_PLUS;
            break;
        case lx_minus:
            token.type = TOK_MINUS;
            break;
        case lx_star:
            token.type = TOK_TIMES;
            break;
        case lx_slash:
            token.type = TOK_DIVIDE;
            break;
        case lx_Iparen:
            token.type = TOK_LPAREN;
            break;
        case lx_rparen:
            token.type = TOK_RPAREN;
            break;
        case lx_eq:
            token.type = TOK_ASSIGN;
            break;
        case lx_semicolon:
            token.type = TOK_SEMICOLON;
            break;
        case lx_colon_eq:
            token.type = TOK_ASSIGN_COLON;
            break;
        case lx_colon:
            token.type = TOK_COLON;
            break;
        case lx_ge:
            token.type = TOK_GE;
            break;
        case lx_gt:
            token.type = TOK_GT;
            break;
        case lx_lt:
            token.type = TOK_LT;
            break;
        case lx_le:
            token.type = TOK_LE;
            break;
        case kw_and:
            token.type = TOK_AND;
            break;
        case kw_or:
            token.type = TOK_OR;
            break;
        case kw_not:
                token.type = TOK_NOT;
                break;
        case lx_string:
            token.type = TOK_STRING;
            std::cout<<"here TOK_STRING"<<std::endl;
           token.text = scannedToken->str_ptr;
            break;
        case lx_float:
            token.type = TOK_FLOAT;
             token.value_f = scannedToken->float_value;
            break;
        case kw_if: {

           token.type=TOK_IF;
            break;
    }
    case kw_fi:

       token.type=TOK_FI;
        break;
    case kw_then:{
    token.text ="then";
       token.type=TOK_THEN;

        break;
    }
    case kw_else:
       token.type=TOK_ELSE;
        break;
    case kw_while:
       token.type=TOK_WHILE;
        break;
    case kw_do:
       token.type=TOK_DO;
        break;
    case kw_od:
       token.type=TOK_OD;
        break;
    case kw_for:
       token.type=TOK_FOR;
        break;
    case kw_to:
       token.type=TOK_TO;
        break;


//        case kw_bool:
//            token.type = TOK_BOOLEAN;
//             token.value = scannedToken->value;
//            break;
        default:
            token.type = TOK_ERROR;
            break;
    }

    delete scannedToken; // Don't forget to clean up!
}
void Parser::wr_error(const QString& msg) {
    scanner.Fd->ReportError(msg.toLocal8Bit().data());
     std::exit(EXIT_FAILURE);
}
AST* Parser::parse_while_loop() {
    expect(TOK_WHILE);
    st.enter_scope();


    get_token();


    AST* condition = parse_expression(1);


    expect(TOK_DO);

    get_token(); //expr

    AST* body = parse_statement();


    get_token();


        expect(TOK_OD);

    st.exit_scope();

    return make_ast_node(ast_while, condition, body);
}




void Parser::expect(TokenType expected) {
    if (token.type != expected) {
        scanner.Fd->ReportError("Unexpected token, expected is %d by got %d\n", expected, token.type);
    }
}
int evaluateAST(AST* node) {


    switch (node->type) {
        case ast_integer:
            return node->f.a_integer.value;
        case ast_plus:{

            return evaluateAST(node->f.a_binary_op.larg) + evaluateAST(node->f.a_binary_op.rarg);
    }
        case ast_minus:
            return evaluateAST(node->f.a_binary_op.larg) - evaluateAST(node->f.a_binary_op.rarg);
    case ast_times:
        return evaluateAST(node->f.a_binary_op.larg) * evaluateAST(node->f.a_binary_op.rarg);

    case ast_divide: {
        if(node->f.a_binary_op.rarg == 0) {
            fatal_error("Can't Divide By zero\n");
        }
        return evaluateAST(node->f.a_binary_op.larg) * evaluateAST(node->f.a_binary_op.rarg);
}

    case ast_eq:
        return evaluateAST(node->f.a_binary_op.larg) == evaluateAST(node->f.a_binary_op.rarg);
    case ast_neq:
        return evaluateAST(node->f.a_binary_op.larg) != evaluateAST(node->f.a_binary_op.rarg);
    case ast_lt:
        return evaluateAST(node->f.a_binary_op.larg) < evaluateAST(node->f.a_binary_op.rarg);
    case ast_le:
        return evaluateAST(node->f.a_binary_op.larg) <= evaluateAST(node->f.a_binary_op.rarg);
    case ast_gt:
        return evaluateAST(node->f.a_binary_op.larg) > evaluateAST(node->f.a_binary_op.rarg);
    case ast_ge:
        return evaluateAST(node->f.a_binary_op.larg) >= evaluateAST(node->f.a_binary_op.rarg);
    case ast_and:
        return evaluateAST(node->f.a_binary_op.larg) && evaluateAST(node->f.a_binary_op.rarg);
    case ast_or:
        return evaluateAST(node->f.a_binary_op.larg) || evaluateAST(node->f.a_binary_op.rarg);
    case ast_not:
        return !evaluateAST(node->f.a_binary_op.larg);
    case ast_var: {
        if(!node->f.a_var.var){
            fatal_error("Variable is undefined");
        }
        return node->f.a_var.var->value;
    }
    case ast_assign: {
                SymbolTableEntry* var_entry = node->f.a_assign.lhs;
                int value = evaluateAST(node->f.a_assign.rhs);
                var_entry->value = value;

                return value;
            }
    case ast_if: {
                int condition = evaluateAST(node->f.a_if.predicate);

                if (condition) {

                    return evaluateAST(node->f.a_if.conseq);
                }
                    else if (node->f.a_if.altern != nullptr) {

                    return evaluateAST(node->f.a_if.altern);
                } else {
                    return 0;  // No alternate provided, returning default value
                }
            }
    case ast_while: {
                int result = 0;
                while (evaluateAST(node->f.a_while.predicate)) {
                    result = evaluateAST(node->f.a_while.body);
                }
                return result;
            }
    case ast_for: {

            SymbolTableEntry* var_entry = node->f.a_for.var;
            AST* lower_bound_node = node->f.a_for.lower_bound;
            AST* upper_bound_node = node->f.a_for.upper_bound;
            AST* body_node = node->f.a_for.body;


            int lower_bound = evaluateAST(lower_bound_node);
            int upper_bound = evaluateAST(upper_bound_node);

            int result = 0;

            for (int i = lower_bound; i <= upper_bound; ++i) {
                var_entry->value = i;
                result = evaluateAST(body_node);
            }

            return result;


    }

    case ast_block: {

           ast_list* stmts = node->f.a_block.stmts;
           int result = 0;

           while (stmts != nullptr) {
               result = evaluateAST(stmts->head);
               cout<<"Result = "<<result<<endl;
               printAST(stmts->head,0);
               stmts = stmts->tail;
           }

           return result;
    }

    default:
            fatal_error("Unknown AST node type in evaluateAST\n");
    }
}

AST* Parser::parse_if_statement() {
    expect(TOK_IF);
    st.enter_scope();
    get_token();

    AST* condition = parse_expression(1);

    expect(TOK_THEN);
    get_token();

    AST* consequence = parse_statement();

    AST* alternate = nullptr;
    get_token();
    if (token.type == TOK_ELSE) {
        get_token();
        alternate = parse_statement();
        get_token();
    }

    expect(TOK_FI);
   // get_token();  // Consume 'fi'
    st.exit_scope();

    return make_ast_node(ast_if, condition, consequence, alternate);
}

AST* Parser::parse_expression(int flag) {
    AST* exp= parse_logical(flag);
    return exp;
}


AST* Parser::parse_for_loop() {
    expect(TOK_FOR);
    get_token();

    // Expect an identifier
    if (token.type != TOK_ID) {
        write_error("Expected identifier after 'for'");
        return nullptr;
    }
    std::string id = token.text;
    get_token();

    // Expect ':='
    if (token.type != TOK_ASSIGN_COLON) {
        write_error("Expected ':=' after identifier in for loop");
        return nullptr;
    }
    get_token();

    AST* start_expr = parse_expression(1);

    // Expect 'to'
    if (token.type != TOK_TO) {
        write_error("Expected 'to' after start expression in for loop");
        return nullptr;
    }
    get_token();

    // Parse the end expression
    AST* end_expr = parse_expression(1);

    // Expect 'do'
    if (token.type != TOK_DO) {
        write_error("Expected 'do' after end expression in for loop");
        return nullptr;
    }
    get_token();

    // Parse the body statement
    AST* body_stmt = parse_statement();
        get_token();
    // Expect 'od'
    if (token.type != TOK_OD) {
        write_error("Expected 'od' at the end of for loop");
        return nullptr;
    }
 // Create AST node for for loop
    st.enter_scope();
    st.PutSymbol(id,evaluateAST(start_expr),ste_var);
    SymbolTableEntry* entry = st.GetSymbol(id);
        st.exit_scope();
    return make_ast_node(ast_for, entry, start_expr, end_expr, body_stmt);
}


AST* Parser::parse_binary(int flag) {
    AST* node = nullptr;
    if (token.type == TOK_INTEGER) {
        node = make_ast_node(ast_integer, token.value);
        get_token();
    } else if (token.type == TOK_LPAREN) {
        get_token();
        node = parse_expression(flag);
        expect(TOK_RPAREN);
        get_token();
    } else if (token.type == TOK_ID) {
        std::string id = token.text;
        get_token();

        SymbolTableEntry* entry = st.GetSymbol(id);
        if (token.type == TOK_ASSIGN_COLON) {
            if (entry->entry_type == 1) {
                write_error("Can't reassign value to constant variable");
                return nullptr;
            }
            get_token();
            AST* expr = parse_expression(flag);
            if (expr == nullptr) {
                write_error("Expected expression after ':='");
                return nullptr;
            }
            if (token.type == TOK_SEMICOLON) {
                //get_token();
                return make_ast_node(ast_assign, entry, expr);
            } else {

                write_error("Expected ';' after assignment");
                return nullptr;
            }
        } else if (entry) {
            return make_ast_node(ast_var, entry, ste_var);
        } else {
            write_error("Undefined variable: " );
            return nullptr;
        }
    } else {

        write_error("Unexpected token in binary expression");
        return nullptr;
    }
    return node;
}

AST* Parser::parse_unary(int flag) {
    if (token.type == TOK_NOT) {
        get_token();
        AST* operand = parse_unary(flag);
        return make_ast_node(ast_not, operand);
    } else {
        return parse_binary(flag);
    }
}

AST* Parser::parse_multiplicative(int flag) {
    AST* left = parse_unary(flag);
    while (token.type == TOK_TIMES || token.type == TOK_DIVIDE) {
        int op_type = token.type;
        get_token();
        AST* right = parse_unary(flag);
        if (op_type == TOK_TIMES) {
            left = make_ast_node(ast_times, left, right);
        } else {
            left = make_ast_node(ast_divide, left, right);
        }
    }
    return left;
}

AST* Parser::parse_additive(int flag) {
    AST* left = parse_multiplicative(flag);
    while (token.type == TOK_PLUS || token.type == TOK_MINUS) {
        int op_type = token.type;
        get_token();
        AST* right = parse_multiplicative(flag);
        if (op_type == TOK_PLUS) {
            left = make_ast_node(ast_plus, left, right);
        } else {
            left = make_ast_node(ast_minus, left, right);
        }
    }
    return left;
}

AST* Parser::parse_relational(int flag) {
    AST* left = parse_additive(flag);
    while (token.type == TOK_EQ || token.type == TOK_NEQ || token.type == TOK_LT ||
           token.type == TOK_LE || token.type == TOK_GT || token.type == TOK_GE) {
        int op_type = token.type;
        get_token();
        AST* right = parse_additive(flag);
        switch (op_type) {
            case TOK_EQ: left = make_ast_node(ast_eq, left, right); break;
            case TOK_NEQ: left = make_ast_node(ast_neq, left, right); break;
            case TOK_LT: left = make_ast_node(ast_lt, left, right); break;
            case TOK_LE: left = make_ast_node(ast_le, left, right); break;
            case TOK_GT: left = make_ast_node(ast_gt, left, right); break;
            case TOK_GE: left = make_ast_node(ast_ge, left, right); break;
        }
    }
    return left;
}

AST* Parser::parse_logical(int flag) {
    AST* left = parse_relational(flag);
    while (token.type == TOK_AND || token.type == TOK_OR) {
        int op_type = token.type;
        get_token();
        AST* right = parse_relational(flag);
        if (op_type == TOK_AND) {
            left = make_ast_node(ast_and, left, right);
        } else {
            left = make_ast_node(ast_or, left, right);
        }
    }
    return left;
}

AST* Parser::parse_declaration(int flag) {
    if (token.type == TOK_VAR) {
        get_token();
        expect(TOK_ID);
        std::string var_name = token.text;
        get_token();
        expect(TOK_COLON);
        get_token();
        expect(TOK_INT);  // Ensure the type is always integer
        get_token();
        expect(TOK_SEMICOLON);
//        get_token();
        SymbolTableEntry* entry = st.GetSymbol(var_name);
        if (entry) {
            write_error("The variable is already declared");
            return nullptr;
        }
        st.PutSymbol(var_name, 0, ste_var);
        return make_ast_node(ast_var_decl, entry, ste_var);
    } else if (token.type == TOK_CONST) {
        get_token();
        expect(TOK_ID);
        std::string const_name = token.text;
        get_token();
        expect(TOK_ASSIGN_COLON);
        get_token();
        AST* expr = parse_expression(flag);

        expect(TOK_SEMICOLON);

        SymbolTableEntry* entry = st.GetSymbol(const_name);
        if (entry) {
            write_error("The variable is already declared");
            return nullptr;
        }
        st.PutSymbol(const_name, evaluateAST(expr), ste_const);
        return make_ast_node(ast_const_decl, entry, expr);
    } else {
        write_error("Expected 'var' or 'const' in declaration");
        return nullptr;
    }
}

AST* Parser::parse_statement() {
    AST* statement = nullptr;

    switch (token.type) {
        case TOK_IF:
            statement = parse_if_statement();
            break;
        case TOK_WHILE:
            statement = parse_while_loop();
            break;
        case TOK_FOR:
            statement = parse_for_loop();
        break;
        case TOK_VAR:
        case TOK_CONST:
            statement = parse_declaration(1);
            break;
    case TOK_Begin:
        statement = parse_block();
        break;
        case TOK_INTEGER:
        case TOK_ID:
        case TOK_LPAREN:
            statement = parse_expression(1);
            break;
        default:
            write_error("Unexpected token in statement parsing");
            get_token();
            break;
    }

    return statement;
}

void Parser::parse() {
    while (token.type != TOK_END) {
        AST* stmt = parse_statement();
        if (stmt != nullptr) {
            printAST(stmt, 0);
            int result = evaluateAST(stmt);
            std::cout << "Result: " << result << std::endl;
        }
        get_token();
    }
}

void Parser::write_error(char *msg) {
    scanner.Fd->ReportError(msg);
}
AST* Parser::parse_block() {
    expect(TOK_Begin);
    st.enter_scope();
    get_token();


    ast_list* stmt_list = nullptr;
    ast_list** stmt_tail = &stmt_list;
    while (token.type != TOK_Endkw && token.type != TOK_END) {
        AST* stmt = parse_statement();
        get_token();
                if (stmt) {
                    ast_list* stmt_node = new ast_list();
                    stmt_node->head = stmt;
                    stmt_node->tail = nullptr;
                    *stmt_tail = stmt_node;
                    stmt_tail = &stmt_node->tail;
        }
        if (token.type == TOK_Endkw) {
            break;
        }
    }

    if(token.type != TOK_Endkw){
        write_error("Expected keyword end at the end of the block");
    }
    get_token();
    st.exit_scope();
    return make_ast_node(ast_block,stmt_list);
}

int main() {
    std::string filename = "C:\\Users\\Administrator\\Documents\\ast\\input.txt"; // Adjust this to your input file path
    FileDescriptor fd("C:\\Users\\Administrator\\Documents\\ast\\input.txt");
    if (!fd.IsOpen()) {
        printf("Can't open %s\n", filename.c_str());
        return 1;
    }
    Parser parser(&fd);
    parser.parse();
    return 0;
}
