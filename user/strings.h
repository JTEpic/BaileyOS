#pragma once
#ifndef STRINGS_H
#define STRINGS_H

// length of chars before \0, array size-1, arr[loc] at \0
int strlen(const char* arr);

// add src to end of dest, can't ensure enough space
void strcat(char* dest,const char* src);

#endif

