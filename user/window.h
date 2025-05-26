#pragma once
#ifndef WINDOW_H
#define WINDOW_H

typedef struct{
    unsigned char *video;
    unsigned int screen_width;
    unsigned int screen_height;
} Screen;

// Define the "class" as a struct containing its data
typedef struct {
    Screen* VGA;
    unsigned int window_height;
    unsigned int window_width;
    unsigned int window_xPos;
    unsigned int window_yPos;
} Window;

// Function to initialize a window
void initializeWindow(Window* instance, Screen* VGA, unsigned int window_xPos, unsigned int window_yPos, unsigned int window_height, unsigned int window_width);

// Updates window position
//void moveWindow(Window* instance, unsigned int window_xPos, unsigned int window_yPos);

// Updates window size
//void resizeWindow(Window* instance, unsigned int window_height, unsigned int window_width);

// Gets 2D -> 1D pixel index
unsigned int getPixelIndex(Window* instance, unsigned int xPos, unsigned int yPos);

//y  x
//1  1 2 3 4 ..
//2
//3
//..
// Gets 2D -> 1D vga text index
unsigned int getTextIndex(Window* instance, unsigned int xPos, unsigned int yPos);

// Gets 2D -> 1D vga color index
unsigned int getColorIndex(Window* instance, unsigned int xPos, unsigned int yPos);

#endif