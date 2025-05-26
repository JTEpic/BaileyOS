#include "editor.h"
#include "lib/strings.h"
#include "lib/serials.h"

#define SPACE_CHAR 0x20 // ASCII space character

void checkLoc(Editor* instance);

void initializeEditor(Editor* instance, Screen* VGA, unsigned int window_xPos, unsigned int window_yPos, unsigned int window_height, unsigned int window_width){
    Window window;
    instance->window=window;
    initializeWindow(&instance->window, VGA, window_xPos, window_yPos, window_height, window_width);
    
    instance->text_xPos=window_xPos;
    instance->text_yPos=window_yPos;
    updateCursor(getPixelIndex(&instance->window,instance->text_xPos,instance->text_yPos));
}

// Where next input will be
void updateEditor(Editor* instance, const char* arr){
    // Set editor text
    if(strlen(arr) == 1){
        instance->window.VGA->video[getTextIndex(&instance->window,instance->text_xPos,instance->text_yPos)] = *(unsigned char*)arr;
        instance->text_xPos++;
    }else if(strcmp(arr, "Spc") == 0){
        instance->window.VGA->video[getTextIndex(&instance->window,instance->text_xPos,instance->text_yPos)] = SPACE_CHAR;
        instance->text_xPos++;
    }else if(strcmp(arr, "Backspc") == 0){
        instance->text_xPos--;
        checkLoc(instance); //If at beginning, removes first char
        instance->window.VGA->video[getTextIndex(&instance->window,instance->text_xPos,instance->text_yPos)] = SPACE_CHAR;
    }else if(strcmp(arr, "CLeft") == 0)
        instance->text_xPos--;
    else if(strcmp(arr, "CRight") == 0)
        instance->text_xPos++;
    else if(strcmp(arr, "CUp") == 0)
        instance->text_yPos--;
    else if(strcmp(arr, "CDown") == 0)
        instance->text_yPos++;
    else if(strcmp(arr, "Enter") == 0)
        instance->text_yPos++;

    checkLoc(instance);
    // Update cursor using pixel loc
    updateCursor(getPixelIndex(&instance->window,instance->text_xPos,instance->text_yPos));
}

// Check if loc is out of window
void checkLoc(Editor* instance){
    //check above window
    if(instance->text_yPos < instance->window.window_yPos){
        instance->text_yPos = instance->window.window_yPos;
    //check below window
    }else if(instance->text_yPos > instance->window.window_yPos+instance->window.window_height){
        instance->text_yPos = instance->window.window_yPos+instance->window.window_height;
    }
    //check left boundary
    if(instance->text_xPos < instance->window.window_xPos){
        instance->text_xPos = instance->window.window_xPos+instance->window.window_width;
        if(instance->text_yPos != instance->window.window_yPos)
            instance->text_yPos--;
    //check right boundary
    }else if(instance->text_xPos > instance->window.window_xPos+instance->window.window_width){
        instance->text_xPos = instance->window.window_xPos;
        if(instance->text_yPos != instance->window.window_yPos+instance->window.window_height)
            instance->text_yPos++;
    }
}