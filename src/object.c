#include "object.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "physics_constants.h"
#include "object_definitions.h"

object init_blank_object()
{
    object o = { DEFAULT_OBJECT_ID, 0, 0, 0.0, 0.0, 0, 0, false };
    return o;
}

object* allocate_new_object_from_params(
    int16_t x, int16_t y, 
    double vel_x, double vel_y, 
    uint16_t w, uint16_t h, 
    bool is_physical)
{
    object* obj = (object*)malloc(sizeof(object));
    
    obj->id = DEFAULT_OBJECT_ID;
    obj->x = x;
    obj->y = y;
    obj->vel_x = vel_x;
    obj->vel_y = vel_y;
    obj->width = w;
    obj->height = h;
    obj->is_physical = is_physical;

    return obj;
}

object* allocate_new_object_from_object(object obj)
{
    return allocate_new_object_from_params(
        obj.x, obj.y,
        obj.vel_x, obj.vel_y,
        obj.width, obj.height,
        obj.is_physical
    );
}


void deallocate_object(object* obj)
{
    free(obj);
}

void render_object_to_screen_as_rectangle(const object* obj, const screen_buff* screen)
{
    for (int y = 0; y < obj->height; y++)
    {
        for (int x = 0; x < obj->width; x++)
        {
            if (obj->x + x < 0 || obj->x + x > screen->width - 1) continue;
            if (obj->y + y < 0 || obj->y + y > screen->height - 1) continue;

            printf("\e[%iB\e[%iC▓", obj->y + y, obj->x + x);
            printf("\e[%iF", obj->y + y);
        }
    }
    fflush(stdout);
}

void update_object_velocity(object* obj)
{
    obj->x += (int)obj->vel_x;
    obj->y += (int)obj->vel_y;

    if (obj->vel_x < MAX_VELOCITY_NEG) obj->vel_x = MAX_VELOCITY_NEG;
    if (obj->vel_x > MAX_VELOCITY_POS) obj->vel_x = MAX_VELOCITY_POS;

    if (obj->vel_y < MAX_VELOCITY_NEG) obj->vel_y = MAX_VELOCITY_NEG;
    if (obj->vel_y > MAX_VELOCITY_POS) obj->vel_y = MAX_VELOCITY_POS;
}

