
#include "symbol.h"
#include <iostream>
void SymbolTable::init(int size) {
    this->size = size;
    slot = new SymbolTableEntry*[size]();
    number_entries = 0;
    number_probes = 0;
    number_hits = 0;
    max_search_dist = 0;
    current_scope = 0;
    scope_stack.push(current_scope);
}

SymbolTable::SymbolTable() {
    init(19); // DEFAULT_SIZE
    fold_case = 0;
}

SymbolTable::SymbolTable(int fold_case_flag) {
    init(19); // DEFAULT_SIZE
    fold_case = fold_case_flag;
}

SymbolTable::~SymbolTable() {
    ClearSymbolTable();
    delete[] slot;
}

void SymbolTable::ClearSymbolTable() {
    for (int i = 0; i < size; ++i) {
        SymbolTableEntry *entry = slot[i];
        while (entry) {
            SymbolTableEntry *next = entry->next;
            delete entry;
            entry = next;
        }
        slot[i] = nullptr;
    }
    number_entries = 0;
    number_probes = 0;
    number_hits = 0;
    max_search_dist = 0;
}

std::string SymbolTable::to_lowercase(const std::string &str) {
    std::string lower_str = str;
    for (size_t i = 0; i < lower_str.length(); ++i) {
        lower_str[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(lower_str[i])));
    }
    return lower_str;
}

unsigned long SymbolTable::ElfHash(const std::string &str) {
    unsigned long h = 0, high;
    for (size_t i = 0; i < str.length(); ++i) {
        h = (h << 4) + static_cast<unsigned char>(str[i]);
        if ((high = h & 0xF0000000)) {
            h ^= high >> 24;
        }
        h &= ~high;
    }
    return h % size;
}

SymbolTableEntry *SymbolTable::GetSymbol(const std::string &str) {
    std::string search_str = fold_case ? to_lowercase(str) : str;
    unsigned long index = ElfHash(search_str);
    SymbolTableEntry *entry = slot[index];
    int probes = 0;

    std::stack<int> temp_scope_stack = scope_stack;
    while (!temp_scope_stack.empty()) {
        int scope = temp_scope_stack.top();
        temp_scope_stack.pop();

        entry = slot[index];
        while (entry) {
            probes++;
            if (strcasecmp(entry->name.c_str(), search_str.c_str()) == 0 && entry->scope == scope) {
                number_hits++;
                return entry;
            }
            entry = entry->next;
        }
    }

    number_probes += probes;
    if (probes > max_search_dist) max_search_dist = probes;
    return nullptr;
}

SymbolTableEntry *SymbolTable::PutSymbol(const std::string &str, int value, ste_entry_type type) {
    number_probes++;
    std::string insert_str = fold_case ? to_lowercase(str) : str;
    SymbolTableEntry *existingEntry = GetSymbol(insert_str);
    if (existingEntry && existingEntry->scope == current_scope) {
            // If the symbol already exists, update its value
            existingEntry->value = value;
            existingEntry->entry_type = type;
            std::cout << "Updated existing symbol: " << insert_str << " with new value " << value << std::endl;

        return existingEntry;
    }
    std::cout << "Inserting symbol: " << insert_str << " with new value " << value << std::endl;

    unsigned long index = ElfHash(insert_str);
    SymbolTableEntry *entry = new SymbolTableEntry;
    entry->name = insert_str;
    entry->value = value;
    entry->entry_type = type;
    entry->scope = current_scope;
    entry->next = slot[index];
    slot[index] = entry;
    number_entries++;
    return entry;
}

void SymbolTable::PrintSymbolStats() {
    printf("Number of entries: %d\n", number_entries);
    printf("Number of probes: %d\n", number_probes);
    printf("Number of hits: %d\n", number_hits);
    printf("Maximum search distance: %d\n", max_search_dist);
}
void SymbolTable::PrintAllSymbols() {
    std::cout << "Entries in Symbol Table:" << std::endl;
    for (int i = 0; i < size; ++i) {
        SymbolTableEntry *entry = slot[i];
        if(entry){
            std::cout << "Name: " << entry->name << ", Scope: " << entry->scope<<"Value: "<<entry->value << ", Type: ";
            switch (entry->entry_type) {
                case ste_var:
                    std::cout << "Variable, \n";
                    break;
                case ste_const:
                    std::cout << "Constant, \n" ;
                    break;
                case ste_routine:
                    std::cout << "Routine, ";
                    break;
                case ste_undefined:
                    std::cout << "Undefined, ";
                    break;
            }
            // std::cout << "Result Type: " << type_names[entry->f.routine.result_type] << std::endl;
            entry = entry->next;

        }
    }
}



void SymbolTable::enter_scope() {
    current_scope++;
    scope_stack.push(current_scope);
}

void SymbolTable::exit_scope() {
    if (!scope_stack.empty()) {
        scope_stack.pop();
        if (!scope_stack.empty()) {
            current_scope = scope_stack.top();
        } else {
            current_scope = 0;
        }
    }
}

int ste_const_value(SymbolTableEntry *e) {
    return e->f.constant.value;
}

const std::string& ste_name(SymbolTableEntry *e) {
    return e->name;
}

j_type ste_var_type(SymbolTableEntry *e) {
    return e->f.var.type;
}
