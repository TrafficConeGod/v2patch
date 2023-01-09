#include "util.h"
#include "game.h"
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>

typedef struct lua_command_execute_this {
    bool success;
    uint8_t _0[3];
    uint8_t b;
    uint8_t _1[15];
    uint32_t c;
    uint32_t d;
} lua_command_execute_this_t;

_Static_assert(sizeof(lua_command_execute_this_t) == 28, "");

extern void* lua_command_execute(lua_command_execute_this_t* this, UNUSED int* param_2) {
    // volatile int x = 10;
    // x++;

    // prevent a crash
    this->d = 0xf;
    this->c = 0;
    this->b = 0;
    this->success = true;

    print_to_debug_console((void*)this + 4, PLEASE_TYPE_SOME_LUA_CODE_MSG, 0x1a);

    return this;
}