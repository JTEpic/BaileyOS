#pragma once
#ifndef IMAGES_H
#define IMAGES_H

// Define the "class" as a struct containing an array
typedef struct {
    //unsigned char arr[ARRAY_SIZE];
    unsigned char* arr;
    unsigned int ARRAY_SIZE;
    //0=grayscale, 1=8bitTo4bit, else grayscale
    unsigned char ARRAY_MODE;
} Images;

// Function to initialize the array
void initializeImg(Images* instance);

// Function to get a pointer to the array (if needed)
unsigned char* getImg(Images* instance);
// Function to get array size
unsigned int getImgSize(Images* instance);
// Function to get array mode
unsigned char getImgMode(Images* instance);

#endif