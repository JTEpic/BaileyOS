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