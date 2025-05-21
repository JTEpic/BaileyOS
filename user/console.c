#include "console.h"
#include "strings.h"
#include "serials.h"

#define SPACE_CHAR 0x20 // ASCII space character

void checkLoc(Console* instance);
void updateCursor();

void initializeConsole(Console* instance, unsigned char* video, unsigned int windowHeight, unsigned int windowWidth){
    instance->video=video;
    instance->HEIGHT=windowHeight;
    instance->WIDTH=windowWidth;
}

// Where next input will be
int textLoc=0;
void updateConsole(Console* instance, const char* arr){
    // Set console text
    if(strlen(arr) == 1){
        instance->video[textLoc] = *(unsigned char*)arr;
        textLoc += 2;
    }else if(strcmp(arr, "Spc") == 0){
        instance->video[textLoc] = SPACE_CHAR;
        textLoc += 2;
    }else if(strcmp(arr, "Backspc") == 0){
        textLoc -= 2;
        checkLoc(instance); //If at beginning, removes first char
        instance->video[textLoc] = SPACE_CHAR;
    }else if(strcmp(arr, "Enter") == 0){
        textLoc += 2*instance->WIDTH;
    }

    checkLoc(instance);
    updateCursor();
}

void checkLoc(Console* instance){
    if(textLoc < 0)
        textLoc = 0;
    else if(textLoc > 2*instance->HEIGHT*instance->WIDTH-2)
        textLoc = 2*instance->HEIGHT*instance->WIDTH-2;
}

void updateCursor()
{
	unsigned int pos = textLoc/2;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (unsigned int) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (unsigned int) ((pos >> 8) & 0xFF));
}