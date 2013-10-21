#ifndef ASSEMBLER_H
#define ASSEMBLER_H

/**
 * puts the bytes in the array at the specified index and resizes if necessary
 * returns the size of the array whether it was changed or not
 */
static size_t putBytes(unsigned short bytes, unsigned char **arr, size_t index, size_t size);

/**
 * resizes the array to be newlen bytes long and copies newlen bytes from the
 * old array to the new one
 */
static void shrink(unsigned char **arr, size_t newlen);

/**
 * prints the data in the array to f as text printing two bytes of binary per
 * line with the most significant bit on the left
 *
 * ex:
 * data = {0xff, 0xff, 0xAA, 0xAA}
 * output:
 * 1111111111111111
 * 1010101010101010
 */
void output(FILE *f, unsigned char *data, size_t datac);

/**
 * assembles the file f by putting the bytecode in the out array and returning
 * the size of the out array.
 *
 * the pointer will be overwritten to an array stored on the heap. Make sure to
 * free it later.
 */
size_t assemble(FILE *f, unsigned char **out);

#endif
