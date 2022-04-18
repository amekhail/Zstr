/**
 * File: zstr.h
 * Author: Adam Mekhail
 * Course: CSc 352
 * The header for the functions for zstr. a zstr
 * is split into 5 different parts, the word length, the total buffer
 * size, the string itself, the null terminator, and additional space in the
 * buffer. The maximium length of the buffer can be 2048 - sizeof(int) * 2 - 1
 * 
 * The buffer will be of sizes 16, 32, 256, 1024, or 2048.
 * The functions that are present creates the zstr, destroys the zstr, appends
 * to the zstr, finds the index of a string in the zstr, counts the occurences
 * of a string in the zstr, compares two zstrs, and finds the substring between
 * a range of letters in the zstr.
 */
#ifndef _ZSTR_H
#define _ZSTR_H
// --- Error codes --
#define ZSTR_OK         0
#define ZSTR_ERROR      100
#define ZSTR_GREATER    1
#define ZSTR_LESS       -1
#define ZSTR_EQUAL      0
// --- zstr types ---
typedef char *  zstr;
typedef int zstr_code;
// --- Globals ---
extern zstr_code zstr_status;
// -- Function headers ---
/**
 * Takes a string and creates space for the zstr object. Copys
 * the strlen, the buffersize, and the string itself into the object
 * and returns the address of the zstr object.
 * The size can be 16, 32, 256, 1024, or 2048. The total number of chars
 * can be (2048 - sizeof(int) * 2 - 1)
 * 
 * @param initial_data the initial string data
 * @return the address of the newly created zstr object starting at the
 * first character in the string 
 */
zstr zstr_create(char * initial_data);

/**
 * Takes the pointer to the zstr starting at the starting character in
 * the string and subtracts 8 from it to get to the begining of the zstr
 * in order to free it from memory
 * 
 * @param to_destroy the zstr to destroy
 */
void zstr_destroy (zstr to_destroy);

/**
 * Appends a string to the base string. If there is no space in the base
 * string to append, the base string will be remalloced to be able to hold the
 * new string.
 * 
 * @param base the base string 
 * @param to_append the string to be added to the base
 */
void zstr_append (zstr * base, zstr to_append);

/**
 * Returns the index where a substring starts in the base string.
 * If it is not found, then it returns ZSTR_LESS or -1.
 * 
 * @param base The string to search
 * @param to_search The substring to find
 * @return the index where the substring starts or -1
 */
int zstr_index (zstr base, zstr to_search);

/**
 * Done in almost the same way as index, just searches the string for
 * the substring. But rather than returning the index it is at, it returns
 * the count for how many times the substring appears in the base
 * 
 * @param base the base to search
 * @param to_search the substring to find
 * @return the count of how many times the substring appears
 */
int zstr_count (zstr base, zstr to_search);

/**
 * @brief Compares two strings to see if they are equal. If the first string
 * is greater than the second, it returns 1, if less, returns -1, and if equal
 * it returns 0
 * 
 * @param x The first string
 * @param y The second string to be compared with
 * @return if the first string is less than, greater than, or equal to the
 * second string
 */
int zstr_compare (zstr x, zstr y);

/**
 * Creates a substring based on the indexes passes in as agruments.
 * Since the spec was very ambiguous about what was wanted (not good
 * practice to be honest, when you give someone a task, especially in industry,
 * you should outline what you want and what should happen in different cases)
 * I just threw an error if the end < begin and if begin was less than 0.
 * Returns a new zstr with the substring
 * @param base the base string
 * @param begin start index
 * @param end end index
 * @return zstr the new substring
 */
zstr zstr_substring (zstr base, int begin, int end);

/**
 * Prints the zstr size, buffer size, and contents
 * as:
 * STRLENGTH: x
 * DATASIZE: y
 * STRING: >text<
 * 
 * @param data the zstr
 */
void zstr_print_detailed(zstr data); 


#endif