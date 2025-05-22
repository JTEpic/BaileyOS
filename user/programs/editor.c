#include "editor.h"
#include "lib/strings.h"
#include "lib/serials.h"

#define SPACE_CHAR 0x20 // ASCII space character

void checkLoc(Editor* instance);

void initializeEditor(Editor* instance, unsigned char* video, unsigned int window_height, unsigned int window_width){
    Window window;
    instance->window=window;
    initializeWindow(&instance->window, video, window_height, window_width);
    
    instance->textLoc=0;
}

// Where next input will be
//int textLoc=0;
void updateEditor(Editor* instance, const char* arr){
    // Set editor text
    if(strlen(arr) == 1){
        instance->window.video[instance->textLoc] = *(unsigned char*)arr;
        instance->textLoc += 2;
    }else if(strcmp(arr, "Spc") == 0){
        instance->window.video[instance->textLoc] = SPACE_CHAR;
        instance->textLoc += 2;
    }else if(strcmp(arr, "Backspc") == 0){
        instance->textLoc -= 2;
        checkLoc(instance); //If at beginning, removes first char
        instance->window.video[instance->textLoc] = SPACE_CHAR;
    }else if(strcmp(arr, "CLeft") == 0){
        instance->textLoc -= 2;
    }else if(strcmp(arr, "CRight") == 0){
        instance->textLoc += 2;
    }else if(strcmp(arr, "CUp") == 0){
        instance->textLoc -= 2*instance->window.window_width;
    }else if(strcmp(arr, "CDown") == 0){
        instance->textLoc += 2*instance->window.window_width;
    }else if(strcmp(arr, "Enter") == 0){
        instance->textLoc += 2*instance->window.window_width;
    }

    checkLoc(instance);
    updateCursor(instance->textLoc/2);
}

// Check if loc is out of window
void checkLoc(Editor* instance){
    if(instance->textLoc < 0)
        instance->textLoc = 0;
    else if(instance->textLoc > 2*instance->window.window_height*instance->window.window_width-2)
        instance->textLoc = 2*instance->window.window_height*instance->window.window_width-2;
}