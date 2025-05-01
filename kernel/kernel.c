#include "kernel.h"
#include <stdbool.h>
//#include <stdio.h>

#define VIDEO_MEMORY 0xb8000
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define GREEN_BG 0x20   // Green background (0010 0000: bg=green, fg=black)
#define GREEN_FG 0x0A   // Green foreground (0000 1010: fg=green, bg=black)
#define BLUE_BG 0x10   // Green background (0010 0000: bg=green, fg=black)
#define BLUE_FG 0x09   // Green foreground (0000 1010: fg=green, bg=black)
#define SPACE_CHAR 0x20 // ASCII space character

void kernel_main(){
    *(char*)VIDEO_MEMORY='Q';
    //printf("skibidi");
    //char* video_memory = (char*) 0xb8000;
    //*video_memory = 'X';

    // Pointer to VGA memory
    unsigned char *video = (unsigned char *)VIDEO_MEMORY;

    bool loop = true;
    bool green=true;
    int updater=0;
    //inf loop
    while(loop){
        // Loop through each cell (80 columns x 25 rows)
        if(updater%(int)50.0E6==0){
            updater=0;
            for (int row = 0; row < SCREEN_HEIGHT; row++){
                for (int col = 0; col < SCREEN_WIDTH; col++){
                    // Calculate index: each cell is 2 bytes (char + attribute)
                    int index = (row * SCREEN_WIDTH + col) * 2;

                    // Set character (e.g., space) and color background
                    if(green){
                        video[index] = SPACE_CHAR;   // Character
                        video[index + 1] = GREEN_BG; // Green background
                        // Alternatively, use GREEN_FG for green text:
                        //video[index + 1] = GREEN_FG;
                    }else{
                        video[index] = SPACE_CHAR;   // Character
                        video[index + 1] = BLUE_BG;
                    }
                }
            }
            green=!green;
        }
        updater++;
    }
    return;
}