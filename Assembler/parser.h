#ifndef PARSER_H
#define PARSER_H

#define KNF 0xffff

typedef enum {A, C} optype;

typedef struct {
    optype type;                /* instruction type */
    unsigned short literal:15;  /* if A it's the literal value @literal */
    unsigned short comp:7;      /* the 7 bit comp field if C type */
    unsigned char dest:3;       /* the 3 bit dest field if C type */
    unsigned char jump:3;       /* the 3 bit jump field if C type */
} instruction;

typedef struct {
    char* name;
    unsigned short data;
} kvp;

typedef struct {
    size_t len;
    kvp list[];
} map;

/* TODO: make these hash tables for speed, we probably won't really need it though and it makes these lists ugly in the code */
extern const map ops;
extern const map jumps;
extern const map dests;

instruction* parseInstruction(char line[]);

/**
 * Removes traling whitespace (puts a \0 at the new end) and returns a pointer
 * to the first non-whitespace char
 */
char* trim(char* str);

/**
 * returns the value associated with the key in the provided map or returns
 * KNF if the key was not found
 */
unsigned short getVal(char* key, const map* list);
#endif
