#include "kernel.h"
#include "images.h"
#include <stdbool.h>

void delay(int count);
void setBackground(char* video,Images* img,unsigned char mode,unsigned int delays);
void colorSwitcher(char* video);
unsigned char getGrayImgChar(unsigned char val);
unsigned char get8bitTo4bitImgChar(unsigned char val);

#define VIDEO_MEMORY 0xb8000
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

#define BLACK_BG 0x0F    // Black background (---- ----: bg=black, fg=white)
#define WHITE_BG 0xF0    // White background (---- 0000: bg=white, fg=black)
#define RED_BG 0x40      // Red background (---- 0000: bg=red, fg=black)
#define GREEN_BG 0x20    // Green background (0010 0000: bg=green, fg=black)
#define BLUE_BG 0x10     // Blue background (---- 0000: bg=blue, fg=black)
#define CYAN_BG 0x30     // Cyan background (---- 0000: bg=cyan, fg=black)
#define MAGENTA_BG 0x50   // Magenta background (---- 0000: bg=magenta, fg=black)
#define BROWN_BG 0x6F    // Brown background (---- ----: bg=brown, fg=white)
#define GRAY_BG 0x7F     // Gray Background (---- ----: bg=gray, fg=white)
#define DARKGRAY_BG 0x8F // DarkGray Background (---- ----: bg=darkgray, fg=white)
#define BRIGHTBLUE_BG 0x90     // BrightBlue background (---- 0000: bg=brightblue, fg=black)
#define BRIGHTGREEN_BG 0xA0    // BrightGreen background (---- 0000: bg=brightgreen, fg=black)
#define BRIGHTCYAN_BG 0xB0     // BrightCyan background (---- 0000: bg=brightcyan, fg=black)
#define BRIGHTRED_BG 0xC0      // BrightRed background (---- 0000: bg=brightred, fg=black)
#define BRIGHTMAGENTA_BG 0xD0  // BrightMagenta background (---- 0000: bg=brightmagenta, fg=black)
#define YELLOW_BG 0xE0         // Yellow background (---- 0000: bg=yellow, fg=black)

#define GREEN_FG 0x0A   // Green foreground (0000 1010: fg=green, bg=black)
#define BLUE_FG 0x09   // Blue foreground (0000 ----: fg=blue, bg=black)

#define SPACE_CHAR 0x20 // ASCII space character

// array size is 2000 (sizeof()), 2000 bytes, rgb per pixel, 80x25
/*static const unsigned char dorm[2000]  = {
    72, 181, 108, 108, 0, 0, 0, 0, 32, 36, 36, 178, 214, 214, 214, 214, 214, 214, 214, 214, 183, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 115, 115, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 115, 115, 115, 115, 72, 76, 147, 113, 149, 108, 151, 151, 73, 69, 69, 69, 69, 
    181, 145, 145, 76, 144, 0, 0, 0, 0, 32, 32, 178, 214, 214, 187, 214, 214, 214, 73, 73, 73, 73, 73, 82, 37, 37, 37, 37, 37, 36, 182, 119, 119, 119, 119, 119, 115, 115, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 115, 115, 115, 115, 115, 119, 119, 114, 40, 113, 151, 151, 73, 73, 69, 69, 68, 
    148, 108, 180, 144, 181, 40, 145, 0, 32, 32, 32, 182, 214, 214, 5, 46, 4, 0, 73, 69, 69, 37, 36, 32, 41, 5, 5, 36, 36, 36, 214, 182, 187, 119, 119, 119, 119, 115, 115, 115, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 115, 115, 119, 119, 119, 151, 41, 151, 151, 151, 155, 187, 105, 73, 73, 73, 73, 
    36, 108, 180, 148, 180, 180, 144, 144, 144, 108, 32, 178, 214, 214, 214, 214, 73, 0, 69, 69, 37, 37, 32, 32, 0, 5, 5, 36, 36, 36, 214, 187, 182, 182, 119, 119, 119, 119, 119, 119, 115, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 115, 115, 115, 115, 119, 119, 151, 109, 145, 108, 146, 145, 187, 187, 187, 73, 73, 68, 68, 69, 
    108, 144, 112, 108, 180, 72, 144, 180, 217, 32, 32, 214, 214, 214, 46, 187, 151, 69, 69, 69, 37, 36, 32, 32, 37, 4, 5, 36, 36, 36, 214, 182, 46, 182, 178, 178, 119, 119, 119, 119, 119, 119, 115, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 115, 115, 115, 115, 119, 119, 119, 114, 114, 78, 114, 147, 113, 113, 145, 150, 145, 73, 73, 69, 68, 68, 
    108, 180, 108, 180, 108, 180, 108, 76, 144, 180, 180, 181, 214, 214, 50, 19, 4, 0, 69, 68, 36, 36, 32, 32, 0, 0, 36, 36, 32, 32, 214, 183, 187, 178, 178, 178, 178, 183, 119, 119, 119, 119, 119, 115, 115, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 115, 115, 115, 115, 119, 119, 119, 151, 151, 151, 151, 77, 73, 109, 109, 76, 112, 108, 36, 73, 73, 105, 73, 72, 
    108, 40, 108, 72, 145, 144, 144, 184, 144, 144, 0, 182, 214, 214, 41, 214, 73, 73, 69, 68, 68, 36, 32, 32, 41, 109, 0, 36, 32, 32, 214, 182, 150, 182, 182, 178, 114, 178, 178, 119, 119, 119, 119, 119, 119, 119, 119, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 119, 119, 119, 151, 151, 151, 114, 114, 114, 150, 113, 181, 145, 145, 145, 182, 145, 73, 105, 72, 72, 105, 
    108, 40, 72, 144, 216, 36, 144, 180, 144, 180, 212, 144, 181, 217, 46, 19, 5, 0, 69, 68, 68, 36, 32, 32, 0, 0, 0, 36, 36, 36, 214, 46, 182, 182, 182, 178, 182, 178, 178, 178, 182, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 115, 119, 119, 119, 119, 151, 151, 78, 77, 146, 113, 109, 151, 182, 150, 145, 182, 112, 144, 108, 144, 105, 105, 105, 104, 105, 
    40, 108, 76, 108, 181, 72, 72, 112, 217, 148, 216, 216, 112, 180, 145, 140, 0, 36, 69, 68, 68, 36, 32, 32, 36, 40, 36, 36, 36, 32, 214, 214, 109, 178, 178, 178, 187, 178, 178, 178, 178, 73, 73, 37, 36, 37, 109, 36, 36, 110, 119, 119, 119, 119, 119, 119, 119, 119, 151, 151, 114, 76, 77, 113, 146, 113, 109, 150, 76, 76, 76, 108, 108, 109, 146, 73, 105, 104, 105, 105, 
    144, 108, 112, 108, 108, 144, 112, 148, 180, 180, 180, 180, 181, 213, 219, 109, 73, 73, 69, 68, 68, 36, 32, 32, 0, 73, 36, 36, 36, 36, 214, 114, 46, 178, 178, 178, 183, 178, 178, 146, 73, 41, 37, 36, 36, 41, 73, 36, 36, 255, 110, 151, 119, 119, 119, 119, 151, 151, 151, 151, 150, 77, 113, 76, 72, 114, 151, 114, 146, 113, 109, 114, 187, 187, 187, 105, 105, 105, 104, 105, 
    181, 36, 148, 148, 72, 76, 72, 36, 0, 104, 140, 108, 144, 180, 217, 217, 144, 0, 73, 68, 68, 36, 32, 32, 32, 0, 0, 36, 36, 36, 210, 182, 141, 178, 178, 178, 255, 178, 178, 73, 73, 37, 36, 36, 36, 37, 36, 36, 36, 146, 142, 110, 151, 151, 151, 151, 151, 151, 151, 119, 150, 119, 151, 151, 151, 151, 114, 113, 113, 113, 182, 187, 186, 150, 114, 105, 105, 105, 104, 105, 
    185, 217, 185, 148, 180, 213, 144, 72, 76, 32, 32, 214, 214, 214, 214, 214, 73, 73, 73, 68, 68, 36, 32, 32, 0, 0, 0, 32, 36, 32, 210, 178, 9, 178, 178, 178, 109, 178, 178, 114, 73, 41, 36, 36, 36, 36, 73, 36, 36, 178, 178, 109, 142, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 155, 119, 109, 151, 113, 151, 150, 108, 145, 150, 187, 187, 105, 105, 105, 105, 105, 
    72, 149, 36, 72, 180, 180, 144, 181, 181, 180, 144, 213, 213, 213, 151, 155, 73, 36, 73, 68, 68, 36, 32, 36, 36, 0, 0, 36, 36, 32, 214, 210, 178, 210, 178, 178, 73, 178, 178, 178, 68, 73, 36, 36, 36, 109, 109, 36, 36, 178, 146, 182, 142, 109, 155, 155, 151, 151, 151, 155, 155, 155, 187, 187, 187, 114, 187, 187, 187, 186, 145, 219, 187, 223, 187, 105, 105, 104, 105, 104, 
    40, 108, 144, 181, 145, 108, 144, 181, 217, 217, 148, 180, 180, 217, 180, 144, 149, 4, 73, 73, 68, 36, 36, 32, 0, 0, 0, 36, 36, 36, 210, 182, 145, 210, 178, 178, 214, 178, 210, 68, 68, 41, 36, 36, 36, 0, 36, 36, 36, 178, 182, 141, 178, 178, 146, 187, 151, 110, 77, 37, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 78, 82, 68, 105, 105, 105, 141, 104, 
    36, 72, 40, 40, 144, 36, 76, 36, 108, 72, 72, 72, 180, 214, 214, 73, 73, 73, 73, 73, 68, 36, 32, 32, 0, 108, 36, 36, 36, 32, 210, 210, 210, 210, 210, 210, 182, 178, 178, 218, 68, 105, 36, 36, 36, 68, 68, 68, 68, 177, 178, 178, 214, 178, 72, 68, 36, 146, 68, 146, 146, 68, 73, 73, 68, 46, 68, 73, 40, 68, 68, 68, 73, 82, 105, 105, 105, 104, 141, 141, 
    218, 72, 181, 145, 108, 36, 72, 181, 185, 109, 144, 144, 180, 213, 144, 36, 72, 109, 73, 73, 68, 36, 32, 0, 73, 109, 109, 32, 32, 36, 210, 105, 178, 210, 210, 210, 109, 210, 210, 178, 146, 109, 36, 36, 36, 141, 68, 68, 68, 210, 178, 178, 182, 182, 141, 68, 68, 109, 68, 73, 73, 68, 72, 105, 68, 109, 68, 68, 73, 68, 68, 68, 68, 36, 68, 105, 105, 141, 104, 137, 
    36, 145, 108, 144, 72, 72, 76, 72, 112, 148, 32, 214, 214, 214, 214, 178, 145, 109, 73, 73, 68, 36, 32, 0, 108, 144, 184, 108, 216, 180, 212, 145, 145, 181, 210, 214, 181, 181, 145, 109, 181, 144, 181, 177, 32, 36, 72, 36, 36, 214, 214, 214, 214, 145, 141, 68, 36, 73, 68, 68, 36, 68, 68, 36, 144, 212, 248, 245, 216, 176, 212, 68, 68, 109, 72, 36, 137, 137, 137, 104, 
    72, 185, 36, 40, 72, 36, 148, 181, 36, 36, 72, 177, 214, 214, 72, 77, 36, 0, 73, 68, 36, 36, 0, 0, 72, 72, 72, 72, 180, 177, 140, 144, 180, 180, 180, 108, 144, 180, 148, 149, 40, 145, 181, 148, 181, 72, 73, 144, 176, 180, 181, 181, 217, 217, 181, 109, 108, 36, 68, 178, 146, 68, 109, 108, 144, 144, 212, 248, 248, 212, 212, 108, 180, 73, 73, 104, 68, 104, 104, 100, 
    0, 0, 0, 0, 32, 0, 0, 32, 32, 32, 32, 214, 214, 214, 178, 182, 182, 73, 36, 36, 36, 0, 0, 0, 0, 0, 0, 32, 0, 36, 104, 72, 145, 104, 32, 0, 0, 144, 144, 217, 40, 40, 0, 0, 108, 0, 0, 32, 32, 177, 177, 109, 145, 214, 250, 36, 68, 36, 72, 72, 32, 104, 68, 109, 36, 0, 32, 36, 73, 0, 0, 32, 32, 0, 36, 100, 36, 72, 104, 104, 
    0, 0, 0, 0, 0, 0, 32, 0, 36, 36, 72, 214, 250, 251, 218, 141, 109, 73, 73, 68, 36, 0, 0, 0, 0, 0, 0, 0, 32, 32, 104, 104, 214, 177, 108, 72, 181, 145, 181, 182, 149, 149, 145, 186, 186, 218, 181, 149, 144, 144, 144, 112, 144, 144, 112, 112, 255, 255, 255, 255, 255, 255, 255, 251, 255, 255, 255, 72, 181, 72, 104, 72, 68, 104, 104, 104, 40, 68, 104, 104, 
    76, 76, 108, 40, 36, 36, 76, 40, 145, 40, 148, 112, 144, 254, 250, 250, 250, 250, 250, 251, 250, 251, 251, 251, 251, 251, 251, 251, 251, 218, 251, 251, 251, 251, 250, 251, 250, 181, 144, 144, 112, 144, 144, 145, 144, 140, 144, 145, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 109, 109, 109, 109, 110, 110, 110, 110, 110, 146, 146, 146, 146, 146, 146, 146, 146, 146, 73, 68, 
    251, 251, 251, 251, 250, 251, 251, 218, 214, 250, 251, 250, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 214, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 73, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 141, 141, 146, 146, 146, 146, 146, 146, 146, 146, 146, 146, 146, 146, 146, 146, 
    182, 250, 251, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 251, 250, 251, 182, 178, 109, 105, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 141, 141, 141, 141, 141, 141, 141, 141, 145, 145, 145, 146, 146, 146, 146, 146, 178, 178, 178, 178, 178, 178, 178, 
    218, 218, 218, 218, 250, 250, 250, 250, 218, 250, 218, 250, 250, 250, 250, 218, 214, 214, 218, 219, 218, 141, 105, 73, 73, 73, 73, 73, 105, 105, 105, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 145, 145, 145, 145, 145, 145, 145, 145, 145, 141, 146, 178, 178, 178, 178, 178, 178, 178, 
    177, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 178, 72, 72, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 105, 105, 109, 109, 109, 109, 109, 109, 109, 109, 105, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 145, 145, 145, 145, 145, 141, 141, 145, 145, 145, 145, 145, 145
};*/

// Max 16 sectors * 512 byte = 8192 bytes
void kernel_main(){
    // Pointer to VGA memory
    unsigned char *video = (unsigned char *)VIDEO_MEMORY;
    
    *(char*)VIDEO_MEMORY='B';
    delay(200000);
    *video = 'T';
    delay(200000);

    //printf("skibidi");
    
    Images img1;
    initializeImg(&img1);

    setBackground(video,&img1,0,1000);

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

//sets background,mode 0=grayscale,mode 1=8bitTo4bit,else grayscale
void setBackground(char* video,Images* img,unsigned char mode,unsigned int delays){
    int pix=0;
    for (int row = 0; row < SCREEN_HEIGHT; row++){
        for (int col = 0; col < SCREEN_WIDTH; col++){
            // Calculate index: each cell is 2 bytes (char + attribute)
            int index = (row * SCREEN_WIDTH + col) * 2;

            // Set character (e.g., space) and color background
            video[index] = SPACE_CHAR;   // Character
            
            if(pix<img->ARRAY_SIZE)
                if(mode==0)
                    video[index + 1] = getGrayImgChar(img->arr[pix]);
                else if(mode==1)
                    video[index + 1] = get8bitTo4bitImgChar(img->arr[pix]);
                else
                    video[index + 1] = getGrayImgChar(img->arr[pix]);
            else
                video[index + 1] = GREEN_BG;
            
            pix++;
            delay(delays);
        }
    }
}

//get grayscale img char
unsigned char getGrayImgChar(unsigned char val){
    if(val>192)
        return WHITE_BG;
    else if(val>128)
        return GRAY_BG;
    else if(val>64)
        return DARKGRAY_BG;
    else
        return BLACK_BG;
}

//get 4 bit img char from 8 bit
unsigned char get8bitTo4bitImgChar(unsigned char val){
    // Extract rrr (bits 7-5), ggg (bits 4-2), bb (bits 1-0)
    unsigned char red = (val >> 5) & 0x07;   // 3 bits: 0-7
    unsigned char green = (val >> 2) & 0x07; // 3 bits: 0-7
    unsigned char blue = val & 0x03;         // 2 bits: 0-3

    // Reduce to 1-bit per component for RGBI (4-bit palette)
    // Threshold: consider value > half of max as "on" (1), else "off" (0)
    unsigned char red_bit = (red > 3) ? 1 : 0;     // 3-bit red (0-7) to 1-bit (0 or 1)
    unsigned char green_bit = (green > 3) ? 1 : 0; // 3-bit green (0-7) to 1-bit (0 or 1)
    unsigned char blue_bit = (blue > 2) ? 1 : 0;   // 2-bit blue (0-3) to 1-bit (0 or 1)

    // Calculate intensity (bright if any component is high)
    bool intensity = (red >= 6 || green >= 6 || blue >= 3) ? 1 : 0;

    // Map to VGA 16-color palette
    if (intensity) {
        // Bright colors (8-15)
        if (red_bit == 0 && green_bit == 0 && blue_bit == 0) {
            return DARKGRAY_BG; // Dark Gray (for low intensity bright)
        } else if (red_bit == 0 && green_bit == 0 && blue_bit == 1) {
            return BRIGHTBLUE_BG; // Light Blue
        } else if (red_bit == 0 && green_bit == 1 && blue_bit == 0) {
            return BRIGHTGREEN_BG; // Light Green
        } else if (red_bit == 0 && green_bit == 1 && blue_bit == 1) {
            return BRIGHTCYAN_BG; // Light Cyan
        } else if (red_bit == 1 && green_bit == 0 && blue_bit == 0) {
            return BRIGHTRED_BG; // Light Red
        } else if (red_bit == 1 && green_bit == 0 && blue_bit == 1) {
            return BRIGHTMAGENTA_BG; // Light Magenta
        } else if (red_bit == 1 && green_bit == 1 && blue_bit == 0) {
            return YELLOW_BG; // Yellow
        } else {
            return WHITE_BG; // White
        }
    } else {
        // Dark colors (0-7)
        if (red_bit == 0 && green_bit == 0 && blue_bit == 0) {
            return BLACK_BG; // Black
        } else if (red_bit == 0 && green_bit == 0 && blue_bit == 1) {
            return BLUE_BG; // Blue
        } else if (red_bit == 0 && green_bit == 1 && blue_bit == 0) {
            return GREEN_BG; // Green
        } else if (red_bit == 0 && green_bit == 1 && blue_bit == 1) {
            return CYAN_BG; // Cyan
        } else if (red_bit == 1 && green_bit == 0 && blue_bit == 0) {
            return RED_BG; // Red
        } else if (red_bit == 1 && green_bit == 0 && blue_bit == 1) {
            return MAGENTA_BG; // Magenta
        } else if (red_bit == 1 && green_bit == 1 && blue_bit == 0) {
            return BROWN_BG; // Brown
        } else {
            return GRAY_BG; // Light Gray
        }
    }
}