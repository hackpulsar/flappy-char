#ifndef CORE_H
#define CORE_H

#include "screen.h"
#include "object.h"

#include "object_definitions.h"
#include "PIPES_SETTINGS.h"

// Contains all the essential data, required for core to functionate
typedef struct
{
    screen_buff* screen_buff;
    object* objects[DEFAULT_OBJECTS_QUANTITY];

    object* pipes[PIPES_QUANTITY];

    bool is_running;

    size_t player_object_id;
} core_data;

// Functions predefines
size_t add_new_object(core_data* data, object* obj);

object* get_object_from_id(const core_data* data, size_t obj_id);

// Initializes core_data instance
core_data core_init(uint16_t screen_width, uint16_t screen_height);

size_t add_new_object(core_data* data, object* obj);

bool delete_object(core_data* data, size_t obj_id);

void core_update(core_data* data);

void core_render(core_data* data);

void core_handle_input(core_data* data);

// Boots up core
void core_start(core_data* data);

// Frees all allocated memory
void core_free_all_memory(core_data* data);

#endif
