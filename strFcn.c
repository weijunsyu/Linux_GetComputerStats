#include <string.h>
#include <stdlib.h>

char* allocStr(char* source) {
    char* ret = malloc(strlen(source) + 1); //allocate mem for string
    strcpy(ret, source); //copy old string source to new string ret
    return ret;
}
