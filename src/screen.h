#ifndef SCREEN_H
#define SCREEN_H

#include <stdlib.h>

// Represents screen stuct
struct screen_buff_s
{
    char* buff;                 // char buffer of all 'pixels'
    u_int16_t width, height;    // dimensions
};
typedef struct screen_buff_s screen_buff;

// Initializes new screen buffer instanse, allocates all memory required
screen_buff* init_screen_buffer(u_int16_t width, u_int16_t height, char default_value);

// Frees all allocated memory of screen_buff instanse
void free_screen_buffer(screen_buff* screen_buff);

#endif
