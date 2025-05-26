#include "window.h"

#define BLUE_BG 0x1F     // Blue background (---- ----: bg=blue, fg=white)
#define CYAN_BG 0x30     // Cyan background (---- 0000: bg=cyan, fg=black)

void initializeWindow(Window* instance, Screen* VGA, unsigned int window_xPos, unsigned int window_yPos, unsigned int window_height, unsigned int window_width){
    instance->VGA=VGA;
    //check if inside screen space or part of a vga draw function
    instance->window_xPos=window_xPos;
    instance->window_yPos=window_yPos;
    instance->window_height=window_height;
    instance->window_width=window_width;

    //Top Bar
    for(int x=window_xPos;x<=window_width+window_xPos;x++){
        VGA->video[getColorIndex(instance, x, window_yPos)] = BLUE_BG;
    }
    //Left Bar
    for(int y=window_yPos+1;y<window_height+window_yPos;y++){
        VGA->video[getColorIndex(instance, window_xPos, y)] = CYAN_BG;
    }
    //Right Bar
    for(int y=window_yPos+1;y<window_height+window_yPos;y++){
        VGA->video[getColorIndex(instance, window_xPos+window_width, y)] = 0x30;
    }
    //Bottom Bar
    for(int x=window_xPos;x<=window_width+window_xPos;x++){
        VGA->video[getColorIndex(instance, x, window_yPos+window_height)] = 0x30;
    }
}

unsigned int getPixelIndex(Window* instance, unsigned int xPos, unsigned int yPos){
    return yPos*instance->VGA->screen_width+xPos;
}

unsigned int getTextIndex(Window* instance, unsigned int xPos, unsigned int yPos){
    return 2*(yPos*instance->VGA->screen_width+xPos);
}

unsigned int getColorIndex(Window* instance, unsigned int xPos, unsigned int yPos){
    return 2*(yPos*instance->VGA->screen_width+xPos)+1;
}