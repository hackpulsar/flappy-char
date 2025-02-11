#include "screen.h"
#include <string.h>

screen_buff* init_screen_buffer(u_int16_t width, u_int16_t height, char default_value)
{
    screen_buff* new_buff = (screen_buff*)malloc(sizeof(screen_buff));      // dynamically allocating memory for screen instanse
    new_buff->buff = (char*)malloc((width * height + 1) * sizeof(char));    // dynamically allocating memory for buffer
    new_buff->width = width;
    new_buff->height = height;

    memset(new_buff->buff, default_value, (width * height) * sizeof(char));
    new_buff->buff[width * height + 1] = '\0';  // setting the terminator char

    return new_buff;
}

void free_screen_buffer(screen_buff* screen_buff)
{
    free(screen_buff->buff);
    free(screen_buff);
}

