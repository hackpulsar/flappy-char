#ifndef OBJECT_H
#define OBJECT_H

#include "screen.h"
#include <stdbool.h>

// Represents an object with basic physic parameters
struct object_s
{
    int16_t id;
    int16_t x, y;
    double vel_x, vel_y;
    uint16_t width, height;
    bool is_physical;
};
typedef struct object_s object;

// Initializes a blank object
object init_blank_object();

// Allocates new object in dynamic memory
object* allocate_new_object_from_params(
    int16_t x, int16_t y, 
    double vel_x, double vel_y, 
    uint16_t w, uint16_t h, 
    bool is_physical
);

object* allocate_new_object_from_object(object obj);

// Deletes object from memory
void deallocate_object(object* obj);

// Renders an object to screen buffer as a rectangle
void render_object_to_screen_as_rectangle(const object* obj, const screen_buff* screen);

// Basic update function for an object
void update_object_velocity(object* obj);

// TODO: render objects as circles too

#endif
