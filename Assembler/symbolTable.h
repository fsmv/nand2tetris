#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#define INITIAL_SIZE 97
#define SNF 0xffff

typedef struct {
    unsigned short address;
    char name[64];
} symbol;

typedef struct {
    size_t tableLen;
    size_t size;
    symbol *symbols;
} symbolTable;

extern const symbol defaultSymbols[];
/* made as extern const intead of #define because it depends on the definition of
   defaultSymbols which is extern const because I can't #define an array */
extern const size_t numDefaultSymbols; 

/**
 * Parses the line for symbols to add, assumes this is being done in order
 *
 * throws an error if an invalid symbol line is passed
 */
void parseSymbols(char *line, symbolTable *st);

/**
 * Replaces the symbols in the string with addresses from the table
 * 
 * modifies the original string and kills the program if the string
 * reqests a symbol not in the table
 */
void replaceSymbols(char *line, symbolTable *st);

/**
 * Initializes the array in st with the default size and adds the default
 * symbols to it
 */
void initDefault(symbolTable *st);

/**
 * hashes the symbols in the src array into the dest table
 * 
 * ignores NULL pointers within the array
 */
void copySymbols(symbolTable *dest, const symbol* src, const size_t srclen);

/**
 * returns the address associated with the symbol or 0xffff if it wasn't found
 */
unsigned short findBySymbol(char *symbol, symbolTable* st);

/**
 * hashes the specified symbol into the hash table and resizes it if it becomes
 * more than half full
 *
 * uses linear probing for collisions
 */
void addSymbol(const char *symbol, unsigned short address, symbolTable *st);

/**
 * doubles the size of the table's array by rehashing the elements
 *
 * assumes that the array is allocated on the stack
 */
void expand(symbolTable *st);

/**
 * returns the hash for a string in a table of size M
 */
size_t hash(const char *key, size_t M);

#endif
