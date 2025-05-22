#pragma once
#ifndef EDITOR_H
#define EDITOR_H

#include "window.h"

// Define the "class" as a struct containing its data
typedef struct {
    Window window;
    int textLoc;
} Editor;

// Function to initialize a editor program
void initializeEditor(Editor* instance, unsigned char* video, unsigned int window_height, unsigned int window_width);

// Updates editor according to the input
void updateEditor(Editor* instance, const char* arr);

#endif