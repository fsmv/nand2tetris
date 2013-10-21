#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "parser.h"
#include "symbolTable.h"

/* TODO: make these hash tables for speed, we probably won't really need it
   though and it makes these lists ugly in the code. I could also just sort
   them and use binary search but maybe that assumes too much. I don't want
   to just hope no one ever unsorts them.
 */

/* it takes up a lot of space, but this is formatted like a table in the
   specification and I'd like to keep it that way */
static const map ops = {
    37, {
    {"0",   42},
    {"1",   63},
    {"-1",  58},
    {"D",   12},
    {"A",   48}, {"M",   112},
    {"!D",  13},
    {"!A",  49}, {"!M",  113},
    {"-D",  15},
    {"-A",  51}, {"-M",  115},
    {"D+1", 31},
    {"1+D", 31},
    {"A+1", 55}, {"M+1", 119},
    {"1+A", 55}, {"1+M", 119},
    {"D-1", 14},
    {"A-1", 50}, {"M-1", 114},
    {"D+A",  2}, {"D+M",  66},
    {"A+D",  2}, {"M+D",  66},
    {"D-A", 19}, {"D-M",  83},
    {"A-D",  7}, {"M-D",  71},
    {"D&A",  0}, {"D&M",  64},
    {"A&D",  0}, {"M&D",  64},
    {"D|A", 21}, {"D|M",  85},
    {"A|D", 21}, {"M|D",  85}
    }
};

static const map jumps = {
    7, {
    {"JGT", 1}, {"JEQ", 2}, {"JGE", 3}, {"JLT", 4},
    {"JNE", 5}, {"JLE", 6}, {"JMP", 7}
    }
};

static const map dests = {
    14, {
    {"M",   1}, {"D",   2}, {"MD",  3}, {"DM",  3}, {"A",   4},
    {"AM",  5}, {"MA",  5}, {"AD",  6}, {"DA",  6}, {"AMD", 7},
    {"ADM", 7}, {"DAM", 7}, {"DMA", 7}, {"MAD", 7}, {"MDA", 7}
    }
};

instruction *parseInstruction(char *line) {
    line = cleanLine(line);
    
    if(strlen(line) == 0) return NULL;

    if(line[0] == '(') return NULL;

    if(line[0] == '@') { //A-type
        if(line[1] <= '9' && line[1] >= '0') {
            instruction* result = malloc(sizeof(instruction));
            result->type = A;

            int imm;
            if(sscanf(line, "@%d", &imm) == 0) {
                fprintf(stderr, "Syntax error: %s\n", line);
                abort();
            }
            
            if(imm >= 1 << 15) {
                fprintf(stderr, "Error, address out of bounds %d\n", imm);
            }

            result->literal = imm;

            return result;
        }
    }else{
        unsigned short comp;
        unsigned char dest, jump;

        parseCType(line, &comp, &dest, &jump);
        
        if(dest == KNF) dest = 0;
        if(jump == KNF) jump = 0;

        if(comp == KERR || dest == KERR || jump == KERR) {
            /* TODO: make a better error message parseCType adds some \0s to line so it won't be the full command anymore */
            fprintf(stderr, "Syntax error: %s\n", line);
            abort();
        }

        instruction *result = malloc(sizeof(instruction));
        result->type = C;
        result->comp = comp;
        result->dest = dest;
        result->jump = jump;

        return result;
    }

    fprintf(stderr, "Syntax error %s\n", line);
    abort();
}

void parseCType(char *line, unsigned short *comp, unsigned char *dest, unsigned char *jump) {
        *dest = 0;
        *jump = 0;

        char *dpos = strchr(line, '=');
        char *jpos = strchr(line, ';');

        if(jpos != NULL) {
            jpos[0] = '\0';
            jpos++;

            jpos = trim(jpos);

            *jump = getVal(jpos, &jumps);
        }else{
            *jump = KNF;
        }

        if(dpos != NULL) {
            dpos[0] = '\0';
            dpos++;

            dpos = trim(dpos);
            line = trim(line);

            *dest = getVal(line, &dests);
            *comp = getVal(dpos, &ops);
        }else{
            *dest = KNF;
            line = trim(line);
            *comp = getVal(line, &ops);
        }
}

int isInstruction(char *line) {
    line = cleanLine(line);
    if(line[0] == '@') return 1;

    unsigned short comp;
    unsigned char dest, jump;
    parseCType(line, &comp, &dest, &jump);
    return comp != KERR;
}

char *cleanLine(char *line) {
    line = trim(line);
    char *comment = strstr(line, "//");
    if(comment != NULL) {
        *comment = '\0'; /* cut off the comment if there is one */
        line = trim(line); /* there might have been more whitespace */
    }

    return line;
}

unsigned short getVal(const char *key, const map *list) {
    int i;
    for(i = 0; i < list->len; i++) {
        if(strcmp(key, list->list[i].name) == 0) {
            return list->list[i].data;
        }
    }

    return KERR;
}

char *trim(char *str) {
    char *end;

    while(isspace(*str)) str++;
    if(*str == '\0') return str;

    end = str + strlen(str) - 1;
    while(end > str && isspace(*end)) end--;

    *(end+1) = '\0';

    return str;
}
