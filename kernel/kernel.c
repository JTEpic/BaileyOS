#include "kernel.h"
#include <stdbool.h>

void delay(int count);
void colorSwitcher(char* video);
char getImgChar(int val);

#define VIDEO_MEMORY 0xb8000
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

#define BLACK_BG 0x0F    // Black background (---- ----: bg=black, fg=white)
#define GRAY_BG 0x7F     // Gray Background (---- ----: bg=gray, fg=white)
#define DARKGRAY_BG 0x8F // DarkGray Background (---- ----: bg=darkgray, fg=white)
#define WHITE_BG 0xF0    // White background (---- 0000: bg=white, fg=black)
#define GREEN_BG 0x20    // Green background (0010 0000: bg=green, fg=black)
#define BLUE_BG 0x10     // Blue background (---- 0000: bg=blue, fg=black)

#define GREEN_FG 0x0A   // Green foreground (0000 1010: fg=green, bg=black)
#define BLUE_FG 0x09   // Blue foreground (0000 ----: fg=blue, bg=black)

#define SPACE_CHAR 0x20 // ASCII space character

void kernel_main(){
    *(char*)VIDEO_MEMORY='Q';
    //printf("skibidi");
    //char* video_memory = (char*) 0xb8000;
    //*video_memory = 'X';

    // Pointer to VGA memory
    unsigned char *video = (unsigned char *)VIDEO_MEMORY;

    bool loop = true;
    int updater=0;
    //inf loop
    while(loop){
        // Loop through each cell (80 columns x 25 rows)
        if(updater%(int)200E6==0){
            updater=0;
            //colorSwitcher(video);
        }
        updater++;
    }
    return;
}

//switch background
bool green=true;
void colorSwitcher(char* video){
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

// Simple blocking delay function
void delay(int count) {
    for (int x=0;x<count;x++) {
        for (int y=0;y<1000;y++) {
            // Empty loop to burn CPU cycles
            asm("nop"); // No-operation instruction
        }
    }
}

//get img char color
char getImgChar(int val){
    if(val>192)
        return WHITE_BG;
    else if(val>128)
        return GRAY_BG;
    else if(val>64)
        return DARKGRAY_BG;
    else
        return BLACK_BG;
}