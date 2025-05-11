#include "kernel.h"
#include "idt.h"
#include "pic.h"
#include "images.h"
#include "serials.h"
#include "keyboard.h"
#include <stdbool.h>

void delay(int count);
void setBackground(unsigned char* video,Images* img,unsigned int delays);
void colorSwitcher(unsigned char* video);
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

// Max 16 sectors * 512 byte = 8192 bytes
void kernel_main(){
    // Pointer to VGA memory
    unsigned char *video = (unsigned char *)VIDEO_MEMORY;
    
    *(char*)VIDEO_MEMORY='B';
    delay(100000);
    *video = 'T';
    delay(100000);

    unsigned char msg[] = "Kernel running\n";
    serial_message(msg);
    unsigned char msg2[] = "Glorious\n";
    serial_message(msg2);
    serial_message("Skibidi\n");
    
    Images img1;
    initializeImg(&img1);
    
    setBackground(video,&img1,200);
    
    // Initialize PIC and IDT
    init_pic();
    init_idt();
    asm volatile("sti"); // Enable interrupts
    clearPIC();

    bool loop = true;
    int updater=0;
    //inf loop
    while(loop){
        const unsigned char scan = get_last_scancode();
        if(scan!=0){
            const char* key = scancode_to_char(scan);
            serial_message(key);
            serial_write('\n');
        }
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
void colorSwitcher(unsigned char* video){
    // Loop through each cell (SCREEN_WIDTH columns x SCREEN_HEIGHT rows)
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
            asm volatile("nop"); // No-operation instruction
        }
    }
}

//sets background,mode 0=grayscale,mode 1=8bitTo4bit,else grayscale
void setBackground(unsigned char* video,Images* img,unsigned int delays){
    // Loop through each cell (SCREEN_WIDTH columns x SCREEN_HEIGHT rows)
    int pix=0;
    for (int row = 0; row < SCREEN_HEIGHT; row++){
        for (int col = 0; col < SCREEN_WIDTH; col++){
            // Calculate index: each cell is 2 bytes (char + attribute)
            int index = (row * SCREEN_WIDTH + col) * 2;

            // Set character (e.g., space) and color background
            video[index] = SPACE_CHAR;   // Character
            
            if(pix<img->ARRAY_SIZE)
                if(img->ARRAY_MODE==0)
                    video[index + 1] = getGrayImgChar(img->arr[pix]);
                else if(img->ARRAY_MODE==1)
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