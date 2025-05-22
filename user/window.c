#include "window.h"

void initializeWindow(Window* instance, unsigned char* video, unsigned int window_height, unsigned int window_width){
    instance->video=video;
    instance->window_height=window_height;
    instance->window_width=window_width;
}