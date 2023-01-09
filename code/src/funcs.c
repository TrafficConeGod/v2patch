#include "util.h"
#include "game.h"
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>

typedef struct command_execute_object {
    bool success;
    uint8_t _0[3];
    uint8_t b;
    uint8_t _1[15];
    uint32_t c;
    uint32_t d;
} command_execute_object_t;

_Static_assert(sizeof(command_execute_object_t) == 28, "");

extern command_execute_object_t* lua_command_execute(command_execute_object_t* this, UNUSED int* param_2) {
    // prevent a crash
    this->d = 0xf;
    this->c = 0;
    this->b = 0;
    this->success = true;

    const char msg[] = "Test";
    print_to_object((void*)this + 4, msg, 4);

    return this;
}