#pragma once
#ifndef WINDOW_H
#define WINDOW_H

// Define the "class" as a struct containing its data
typedef struct {
    unsigned char* video;
    unsigned int window_height;
    unsigned int window_width;
    //unsigned int position;
} Window;

// Function to initialize a window
void initializeWindow(Window* instance, unsigned char* video, unsigned int window_height, unsigned int window_width);

// Updates window position
//void updateWindow(Window* instance, unsigned int pos);

#endif