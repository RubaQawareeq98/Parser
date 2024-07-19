#ifndef SYMBOL_H
#define SYMBOL_H

#include <cstdio>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <stack>
#include <string>

#ifdef _WIN32
#include <string.h>
#define strcasecmp _stricmp
#else
#endif

typedef enum {
    type_integer,
    type_string,
    type_boolean,
    type_float,
    type_none
} j_type;

static const char *type_names[] = {"integer", "string", "boolean", "float", "none"};

typedef enum {
    ste_var,
    ste_const,
    ste_routine,
    ste_undefined
} ste_entry_type;

struct SymbolTableEntry {
    std::string name;
    int value;
    SymbolTableEntry *next;
    ste_entry_type entry_type;
    union {
        struct { j_type type; } var;
        struct { int value; } constant;
        struct { j_type result_type; } routine;
    } f;
    int scope;

    SymbolTableEntry() : next(nullptr), entry_type(ste_undefined), scope(0) {}
};

class SymbolTable {
private:
    SymbolTableEntry **slot;
    int fold_case;
    int size;
    int number_entries;
    int number_probes;
    int number_hits;
    int max_search_dist;
    std::stack<int> scope_stack;
    int current_scope;

    unsigned long ElfHash(const std::string &str);
    void init(int size);
    std::string to_lowercase(const std::string &str);
public:
    SymbolTable();
    SymbolTable(int fold_case_flag);
    ~SymbolTable();
    void ClearSymbolTable();
    SymbolTableEntry *GetSymbol(const std::string &str);
    SymbolTableEntry *PutSymbol(const std::string &str, int value, ste_entry_type type);
    void PrintSymbolStats();
    void enter_scope();
    void exit_scope();
    void PrintAllSymbols();
};

int ste_const_value(SymbolTableEntry *e);
const std::string& ste_name(SymbolTableEntry *e);
j_type ste_var_type(SymbolTableEntry *e);

#endif // SYMBOL_H
