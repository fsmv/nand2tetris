#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parser.h"

size_t putBytes(unsigned short bytes, unsigned char** arr, size_t index, size_t size) {
    int result = size;

    if(index + 2 > size) {
        char* temp = malloc(2*size);
        memcpy(temp, *arr, size);
        free(*arr);
        *arr = temp;

        result = 2*size;
    }
    
    (*arr)[index] = (unsigned char) (bytes >> 8);
    (*arr)[index+1] = (unsigned char) (bytes & 0xff);

    return result;
}

void shrink(unsigned char** arr, size_t newlen) {
    char* temp = malloc(newlen);
    memcpy(temp, *arr, newlen);
    free(*arr);
    *arr = temp;
}

void output(FILE* f, unsigned char* data, size_t datac) {
    int i;
    for(i = 0; i < datac/2; i++) {
        unsigned short code = (data[2*i] << 8) + (data[2*i+1]);
        char temp[18];
        temp[16] = '\n';
        temp[17] = '\0';
        
        int j;
        for(j = 0; j < 16; j++) {
            temp[j] = (code >> 15-j) & 1 ? '1' : '0';
        }

        fprintf(f, "%s", temp);
    }
}

size_t assemble(FILE* f, unsigned char* out[]) {
    char buff[256];
    int i,n = 0;
    int c;

    //start with room for 32 instructions
    unsigned char* result = malloc(64);
    size_t rsize = 64;

    do {
        c = fgetc(f);
        
        if(c != '\n'){
            if(i >= 256){
                fprintf(stderr, "Line %d too long (max 256), buffer overflow\n", n);
                abort();
            }

            buff[i] = c;
            i++;
        }else{
            buff[i] = '\0';
            i = 0;

            instruction* op;
            op = parseLine(buff);

            if(op != NULL) {
                unsigned short data;
                if(op->type == A) {
                    data = op->literal;
                    data = data & 0x7fff;
                }else{
                    data = (0x7 << 13) + (op->comp << 6) + (op->dest << 3) + (op->jump);
                }

                rsize = putBytes(data, &result, 2*n, rsize);
                free(op);

                n++;
            }
        }
    }while(c != EOF);

    shrink(&result, 2*n);
    //output(stdout, result, 2*n);
    *out = result;
    return 2*n;
}

int main(int argc, char* argv[]) {
    if(argc >= 2) {
        FILE* infile;
        infile = fopen(argv[1], "r");
        size_t outlen;
        unsigned char* out;
        outlen = assemble(infile, &out);
        fclose(infile);

        FILE* outfile;
        char outfilename[64];
        char ext[8];
        sscanf(argv[1], "%[^.].%s", outfilename, ext);
        strcpy(ext, "hack");

        sprintf(outfilename, "%s.%s", outfilename, ext);
        outfile = fopen(outfilename, "w");

        output(outfile, out, outlen);
        fclose(outfile);
        free(out);
    }else{
        printf("Usage: %s [filename]\n", argv[0]);
    }
}
