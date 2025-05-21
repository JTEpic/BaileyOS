#pragma once
#ifndef CONSOLE_H
#define CONSOLE_H

// Define the "class" as a struct containing an array
typedef struct {
    unsigned char* video;
    unsigned int HEIGHT;
    unsigned int WIDTH;
    //Window window;
} Console;

// Function to initialize a console
void initializeConsole(Console* instance, unsigned char* video, unsigned int windowHeight, unsigned int windowWidth);

// Updates console according to the input
void updateConsole(Console* instance, const char* arr);

#endif