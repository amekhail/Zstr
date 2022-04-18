/**
 * File: zstr.c
 * Author: Adam Mekhail
 * Course: CSc 352
 * To implement the functions for zstr. a zstr
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
#include<stdio.h>
#include<stdlib.h>
#include"zstr.h"

int zstr_space(char * inital_data);
int myStrLen(char * str);
int compare(char * hayStack, char * needle);
int myStrStrForIndex(char * hayStack, char * needle);
char * myStrStrForCount(char * hayStack, char * needle);

// initially have the status to ok
zstr_code zstr_status = ZSTR_OK;

// --- Function implementaions (see h file for comments) ---

zstr zstr_create(char * initial_data) {
    int mallocatedSpace = zstr_space(initial_data);
    int len = myStrLen(initial_data);
    int * newStr = malloc(mallocatedSpace);
    if (newStr == NULL) { // check if error allocating space
        fprintf(stderr, "Error allocating space\n");
        zstr_status = ZSTR_ERROR;
        return NULL;
    }
    // copy data over
    newStr[0] = len;
    newStr[1] = mallocatedSpace;
    // make a copy so newStr can point to the begining still
    int * temp = newStr + 2;
    char * dataStart = (char *) temp;
    // individually copy the letters over
    for (int i = 0; i < len; i++) {
        *dataStart = initial_data[i];
        dataStart++;
    }
    // add null terminator
    *dataStart = '\0';
    // cast back to zstr and return
    newStr = temp; // set to the start of the string
    return (zstr)newStr;
}

void zstr_destroy (zstr to_destroy) {
    // subtract 8 bytes from start index to get to begining of zstr
    to_destroy -= 8; 
    free(to_destroy);
}

void zstr_append (zstr * base, zstr to_append) {
    int *appendTemp = (int *)(to_append - 8), *baseTemp = (int *) (*base - 8);
    int baseLen = baseTemp[0], baseBuffer = baseTemp[1] - 9;
    int appendLen = appendTemp[0];
    int newLen = baseLen + appendLen + 1;
    char * temp = (char *)(*base), * temp2 = (char*)(to_append);
    /* pointer *magic* */
    if (baseLen + appendLen > baseBuffer) { // if chars to add exceed total buf
        char * copy = malloc(newLen + 1);
        if (copy == NULL) { // make a copy of the new string
            fprintf(stderr, "Error allocating space. Error: %d\n", ZSTR_ERROR);
            zstr_status = ZSTR_ERROR;
            return;
        }
        char * copyPtr = copy;
        for (int i = 0; i < baseLen; i++) {
            *copyPtr = *temp;
            copyPtr++; temp++;
        }
        int i = 0;
        for (; i < appendLen; i++) {
            *copyPtr = *temp2;
            copyPtr++; temp2++;
        }
        *copyPtr = '\0';
        zstr_destroy(*base);
        *base = zstr_create(copy); // make new base with cat string
        free(copy); // free the copy
    } else { // if theres enough space
        int i = 0;
        temp = (temp + baseLen);
        for (; i < appendLen; i++) {
            *temp = *temp2;
            temp++; temp2++;
        }
        temp = '\0';
        baseTemp[0] = newLen - 1;
    }
}

int zstr_index (zstr base, zstr to_search) {
    char * baseStart = (char *)base, *searchStart = (char*)to_search;
    return myStrStrForIndex(baseStart, searchStart);
}

int zstr_count (zstr base, zstr to_search) {
    char * baseStart = (char *)base, *searchStart = (char*)to_search;
    int count = 0; // occurence count
    while(baseStart != NULL) {
        // increment each letter for search
        baseStart = myStrStrForCount(baseStart, searchStart);
        if (baseStart != NULL) {
            count++;
        }
    }
    return count;
}

int zstr_compare (zstr x, zstr y) {
    char * baseStart = (char *)x, *searchStart = (char*)y;
    for (int i = 0; ; i++) {
        if (baseStart[i] != searchStart[i]) {
           if (baseStart[i] > searchStart[i]) {
               return ZSTR_GREATER;
           } else {
               return ZSTR_LESS;
           }
        }
        if (baseStart[i] == '\0') {
            return ZSTR_EQUAL;
        }
    }
}

zstr zstr_substring (zstr base, int begin, int end) {
    char * baseStart = (char *)base;
    if ((begin < 0) || (end < begin) || (base == NULL)) {
        fprintf(stderr, "Substring Error. Error code: %d", ZSTR_ERROR);
        zstr_status = ZSTR_ERROR;
        return NULL;
    }
    int len = end - begin;
    char * dest = malloc(len + 1);
    char * destCpy = dest;
    // copy all chars in range
    for (int i = begin; i < end && (baseStart[i] != '\0'); i++) {
        *dest = baseStart[i];
        dest++;
    }
    dest = '\0'; // set null terminator
    return zstr_create(destCpy);
}

void zstr_print_detailed(zstr data) {
    int *temp = (int *)(data - 8);
    printf("STRLENGTH: %d\n", temp[0]);
    printf(" DATASIZE: %d\n", temp[1]);
    printf("   STRING: >%s<\n", data);
}

// --- Private Helper Functions ---
// NOTE: These functions are adaptations of functions that i've written
// when i took ECE175 and ECE275. Since I wrote them, I am going to reuse
// them and adapt them as needed

/**
 * Returns the strlen of a given string since we are not allowed
 * to use stdlib functions. This is pretty straight forward, just counts
 * all the chars up until the '\0'
 * 
 * @param str the string
 * @return the char count
 */
int myStrLen(char * str) {
    int count = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        count++;
    }
    return count;
}

/**
 * Calculates the amount of space to allocated for a zstr
 * depending on the strlen
 * 
 * @param inital_data the string to be made into a zstr
 * @return the size the zstr should be
 */
int zstr_space(char * inital_data) {
    int len = myStrLen(inital_data);
    int totalSize = len + 9;
    if (totalSize <= 16) {
        return 16;
    } else if (totalSize <= 32) {
        return 32;
    } else if (totalSize <= 256) {
        return 256;
    } else if (totalSize <= 1024) {
        return 1024;
    } else {
        return 2048;
    }
}
/**
 * Checks if a substring is equal to a base string at any given starting index
 * If not equal, it will return 0, otherwise it will return 1
 * 
 * @param hayStack the base string starting at any index
 * @param needle the substring to find
 * @return if equal or not
 */
int compare(char * hayStack, char * needle) {
    while (*hayStack && *needle) { // while both strings arent null
        if (*hayStack != *needle) {
            return 0; // strings not equal
        }
        hayStack++; needle++;
    }
    return (*needle == '\0'); // substring found since reached the end of str
}

/**
 * Returns the index that the first occurence was found searching for the
 * substring at a given letter in the base string
 * 
 * @param hayStack the base string
 * @param needle the string to be found
 * @return the index where the word is found
 */
int myStrStrForIndex(char * hayStack, char * needle) {
    int count = 0;
    while(*hayStack != '\0') {
        if ((*hayStack == *needle) && compare(hayStack,needle)) {
            return count;
        }
        hayStack++;
        count++;
    }
    return ZSTR_LESS;
}

/**
 * Similar to myStrStrForIndex, it takes a string at a given start, and
 * searches for a substring, if not found it returns null, if found it returns
 * the next spot in the string
 * 
 * @param hayStack the base string
 * @param needle the string to be found
 * @return the next position in the string if a substring is found or null
 * if the substring is not found
 */
char * myStrStrForCount(char * hayStack, char * needle) {
    while(*hayStack != '\0') {
        if ((*hayStack == *needle) && compare(hayStack,needle)) {
            hayStack++; // increment to start at next spot
            return hayStack;
        }
        hayStack++; // increment anyways to search
    }
    return NULL;
}