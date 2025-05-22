#include "strings.h"

int strlen(const char* arr){
    int len=0;
    while(arr[len] != '\0') len++;
    return len;
}

void strcat(char* dest,const char* src){
    int start=strlen(dest);
    // includes \0 as well
    for(int x=0;x<strlen(src)+1;x++){
        dest[start+x]=src[x];
    }
}

int strcmp(const char* arr1, const char* arr2){
    while (*arr1 != '\0' && *arr2 != '\0') {
        if (*arr1 != *arr2) {
            return (unsigned char)(*arr1) - (unsigned char)(*arr2);
        }
        arr1++;
        arr2++;
    }
    return (unsigned char)(*arr1) - (unsigned char)(*arr2);
}