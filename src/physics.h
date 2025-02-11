#ifndef PHYSICS_H
#define PHYSICS_H

#include "object.h"
#include "physics_constants.h"

void apply_gravity_to_object(object* obj)
{
    obj->vel_y -= GRAVITY;
}

bool AABB_Collision(const object* obj1, const object* obj2)
{
    if (obj1->x < obj2->x + obj2->width &&
        obj1->x + obj1->width > obj2->x &&
        obj1->y < obj2->y + obj2->height &&
        obj1->y + obj1->height > obj2->y)
    {
        return true;
    }

    return false;
}

#endif
