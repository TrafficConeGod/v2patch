#include "util.h"
#include <stdint.h>

extern void* lua_command_execute(void* this, UNUSED int* param_2) {
    // volatile int x = 10;
    // x++;

    // prevent a crash
    *(uint32_t*)(this + 0x18) = 0xf;
    *(uint32_t*)(this + 0x14) = 0;
    *(uint8_t*)(this + 0x4) = 0;
    *(uint8_t*)this = 1;

    return this;
}

extern void unused_function() {
    return;
}