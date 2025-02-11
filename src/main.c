#include "core.h"

int main()
{
    core_data data = core_init(64, 32);
    core_start(&data);
    core_free_all_memory(&data);

    return 0;
}
