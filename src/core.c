#include "core.h"

#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>

#include "physics.h"
#include "PIPES_SETTINGS.h"

object* get_object_from_id(const core_data* data, size_t obj_id)
{
    for (int i = 0; i < DEFAULT_OBJECTS_QUANTITY; i++)
    {
        if (data->objects[i]->id == obj_id)
            return data->objects[i];
    }

    return allocate_new_object_from_object(init_blank_object());
}

core_data core_init(uint16_t screen_width, uint16_t screen_height)
{
    core_data data;
    data.screen_buff = init_screen_buffer(screen_width + 1, screen_height + 1, '.');
    data.is_running = 1;

    // Setting up default objects
    for (int i = 0; i < DEFAULT_OBJECTS_QUANTITY; i++)
        data.objects[i] = allocate_new_object_from_object(init_blank_object());

    // Setting up a console
    printf("\e[8;%i;%it", screen_height + 1, screen_width + 1); // Resize console window
    system("clear");

    // Hide cursor
    printf("\e[?25l");

    // Add player object
    data.player_object_id = add_new_object(&data, allocate_new_object_from_params(4, 4, 0, 0, 4, 2, true));

    // Add pipes
    srand((unsigned) time(NULL));
    for (int i = 0; i < PIPES_QUANTITY; i++)
    {
            size_t pipe_id = add_new_object(
                    &data, 
                    allocate_new_object_from_params(
                        screen_width + (i * DISTANCE_BETWEEN_PIPES), 
                        rand() % (screen_height - PIPE_MIN_HEIGHT) + PIPE_OFFSET_FROM_TOP, 
                        -1.0, 0.0,
                        3, screen_height, 
                        false
                    )
            );

        data.pipes[i] = get_object_from_id(&data, pipe_id); 
    }

    return data;
}

size_t add_new_object(core_data* data, object* obj)
{
    static size_t last_id = 0;

    // Loop through objects array until we find an empty slot
    for (int i = 0; i < DEFAULT_OBJECTS_QUANTITY; i++)
    {
        if (data->objects[i]->id == DEFAULT_OBJECT_ID)
        { // free slot found
            obj->id = last_id++;
            data->objects[i] = obj;

            return obj->id;
        }
    }

    return DEFAULT_OBJECT_ID;
}

bool delete_object(core_data* data, size_t obj_id)
{
    for (int i = 0; i < DEFAULT_OBJECTS_QUANTITY; i++)
    {
        if (data->objects[i]->id == obj_id)
        {
            deallocate_object(data->objects[i]);
            data->objects[i] = allocate_new_object_from_object(init_blank_object());
            return true;
        }
    }

    return false;
}

void core_update(core_data* data)
{
    for (int i = 0; i < DEFAULT_OBJECTS_QUANTITY; i++)
    {
        if (data->objects[i]->id != DEFAULT_OBJECT_ID && data->objects[i]->is_physical)
            apply_gravity_to_object(data->objects[i]);

        update_object_velocity(data->objects[i]);
    }

    // Wrap pipes
    for (int i = 0; i < PIPES_QUANTITY; i++)
    {
        if (data->pipes[i]->x + data->pipes[i]->width < -1)
            data->pipes[i]->x = data->screen_buff->width + 2;

        // Check for collision with player
        if (AABB_Collision(data->pipes[i], get_object_from_id(data, data->player_object_id)))
        {
            data->is_running = false;
        }
    }
}

void core_render(core_data* data)
{
    // Render background
    for (int i = 0; i < data->screen_buff->height + 1; i++)
    {
        for (int j = 0; j < data->screen_buff->width; j++)
        {
            printf("\e[%iB\e[%iC%c", i, j, data->screen_buff->buff[i * data->screen_buff->width + j]);
            printf("\e[%iF", i);
        } 
    }

    // Move cursor back to top
    printf("\e[%iA", data->screen_buff->height + 2);

    for (int i = 0; i < 16; i++)
    {
        if (data->objects[i]->id != DEFAULT_OBJECT_ID)
        {
            render_object_to_screen_as_rectangle(data->objects[i], data->screen_buff);
        }
    }
}

void core_handle_input(core_data* data)
{
    // Read keyboard
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
    if (FD_ISSET(STDIN_FILENO, &fds)) {
        int ch = getchar();
        if (ch == ' ')
        { // Spacebar is pressed
            get_object_from_id(data, data->player_object_id)->vel_y -= 5.0;
        }
    }
}

void core_start(core_data* data)
{
    // !!! I have no idea what this code does, i found it on GitHub lol
    // Switch to canonical mode, disable echo
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while (data->is_running)
    {
        core_handle_input(data);
        core_update(data);
        core_render(data);

        usleep(5 * 1000000 / 60); // delay between frames
    }
    
    printf("\e[%iB\e[%iC Game Over! ", data->screen_buff->height / 2, data->screen_buff->width / 2 - 5);
    printf("\e[%iF", data->screen_buff->height/ 2);
    fflush(stdout);
    getchar();

    // Show cursor
    printf("\e[?25h");

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

void core_free_all_memory(core_data* data)
{
    free_screen_buffer(data->screen_buff);
    for (int i = 0; i < DEFAULT_OBJECTS_QUANTITY; i++)
        deallocate_object(data->objects[i]);
}
