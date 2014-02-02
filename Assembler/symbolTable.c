#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parser.h"
#include "symbolTable.h"

void parseSymbols(char *line, symbolTable *st) {
    static unsigned short labelAddr = 0;
    
    if(isInstruction(line)) {
        labelAddr++;
        return;
    }
    
    line = cleanLine(line);
    if(strlen(line) == 0) {
        return;
    }

    if(line[0] == '(') {
        if(line[1] != '\0' && !(line[1] <= '9' && line[1] >= '0')) {
            char *end = strchr(line, ')');
            if(end != NULL) {
                *end = '\0';
                line++;

                addSymbol(line, labelAddr, st);
                return;
            }
        }
    }

    fprintf(stderr, "Sytax error on symbol, addr: %d\n", labelAddr);
    abort();
}

void replaceSymbols(char *line, symbolTable *st) {
    static unsigned short varaddr = 16;
    
    line = cleanLine(line);
    if(line[0] == '@') {
        if(line[1] != '\0' && !(line[1] <= '9' && line[1] >= '0')) {
            unsigned short addr = findBySymbol(line+1, st);
            
            if(addr == SNF) {
                addr = varaddr;
                addSymbol(line+1, varaddr, st);
                varaddr++;
            }

            sprintf(line+1, "%d", addr);
        }
    }
}

void initDefault(symbolTable *st) {
    st->symbols = calloc(INITIAL_SIZE, sizeof(symbol));
    st->tableLen = INITIAL_SIZE;
    st->size = 0;

    copySymbols(st, defaultSymbols, numDefaultSymbols);
}

void copySymbols(symbolTable *dest, const symbol *src, const size_t srclen) {
    int i;
    for(i = 0; i < srclen; i++) {
        if(strlen(src[i].name) != 0) {
            addSymbol(src[i].name, src[i].address, dest);
        }
    }
}

unsigned short findBySymbol(char *name, symbolTable *st) {
    int h = hash(name, st->tableLen);
    while(strlen(st->symbols[h].name) != 0 && strcmp(st->symbols[h].name, name))
        h = h + 1 % st->tableLen;

    if(strlen(st->symbols[h].name) == 0) {
        return SNF;
    }

    return st->symbols[h].address;
}

void addSymbol(const char *name, unsigned short address, symbolTable *st) {
    int h = hash(name, st->tableLen);
    while(strlen(st->symbols[h].name) != 0)
        h = (h + 1) % st->tableLen;
    
    symbol sym;
    sym.address = address;
    strcpy(sym.name, name);
    
    st->symbols[h] = sym;

    if(st->size++ >= st->tableLen/2) expand(st);
}

void expand(symbolTable *st) {
    symbol *tableVals = st->symbols;
    size_t oldLen = st->tableLen;

    /* double the table size and add 1 on expansion */
    st->tableLen = (st->tableLen * 2) + 1;
    st->symbols = calloc(st->tableLen, sizeof(symbol));
    st->size = 0;

    copySymbols(st, tableVals, oldLen);

    free(tableVals);
}

size_t hash(const char *key, size_t M) {
    /* a and b are just random constants for the "random" number generation */
    int h, a = 31415, b = 27183;

    for(h = 0; *key != 0; key++)
        h = (a*h + *key) % M;
    
    return (h < 0) ? (h + M) : h;
}

const symbol defaultSymbols[] = {
      {0, "SP"},  {1, "LCL"},  {2, "ARG"},
    {3, "THIS"}, {4, "THAT"},   {0, "R0"},
      {1, "R1"},   {2, "R2"},   {3, "R3"},
      {4, "R4"},   {5, "R5"},   {6, "R6"},
      {7, "R7"},   {8, "R8"},   {9, "R9"},
    {10, "R10"}, {11, "R11"}, {12, "R12"},
    {13, "R13"}, {14, "R14"}, {15, "R15"},
    {0x4000, "SCREEN"},   {0x6000, "KBD"}
};

const size_t numDefaultSymbols = 23;
