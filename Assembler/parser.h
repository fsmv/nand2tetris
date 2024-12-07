#ifndef PARSER_H
#define PARSER_H

#define KNF  0xff
#define KERR 0xef

typedef enum {A, C} optype;

typedef struct {
    optype type;                /* instruction type */
    unsigned short literal:15;  /* if A it's the literal value @literal */
    unsigned short comp:7;      /* the 7 bit comp field if C type */
    unsigned char dest:3;       /* the 3 bit dest field if C type */
    unsigned char jump:3;       /* the 3 bit jump field if C type */
} instruction;

typedef struct {
    char *name;
    unsigned short data;
} kvp;

typedef struct {
    size_t len;
    kvp list[];
} map;


/**
 * Parses an instruction returning NULL for a blank line (or a comment) or
 * a heap-allocated instruction with the values correctly filled.
 *
 * Prints an error message and kills the program if an invalid command is parsed
 */
instruction* parseInstruction(char *line);

/**
 * Parses a c-type instruction (dest=comp;jump) where the fields can be filled
 * only by elements of the respective maps and both dest and comp can be ommited
 * 
 * Assumes the string is a C-type instruction with no whitespace or comments
 * surrounding it (things like dest = comp;\tjump are allowed but not
 * dest=comp;jump\t)
 *
 * Sets comp, dest, and jump to their codes or KNF if that field was not present
 * and KERR if invalid
 */
void parseCType(char *line, unsigned short *comp, unsigned char *dest, unsigned char *jump);

/**
 * returns whether the line is a valid instruction or not
 */
int isInstruction(char *line);

/**
 * removes comments and trims the line
 */
char *cleanLine(char *line);

/**
 * Removes traling whitespace (puts a \0 at the new end) and returns a pointer
 * to the first non-whitespace char
 */
static char *trim(char *str);

/**
 * returns the value associated with the key in the provided map or returns
 * KERR if the key was not in the map
 */
static unsigned short getVal(const char *key, const map *list);

#endif
