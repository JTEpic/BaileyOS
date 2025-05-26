#pragma once
#ifndef EDITOR_H
#define EDITOR_H

#include "window.h"

// Define the "class" as a struct containing its data
typedef struct {
    Window window;
    unsigned int text_xPos;
    unsigned int text_yPos;
} Editor;

// Function to initialize a editor program
void initializeEditor(Editor* instance, Screen* VGA, unsigned int window_xPos, unsigned int window_yPos, unsigned int window_height, unsigned int window_width);

// Updates editor according to the input
void updateEditor(Editor* instance, const char* arr);

#endif