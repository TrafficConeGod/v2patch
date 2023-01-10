#include "util.h"
#include "game.h"
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct printable_object {
    uint32_t data[6];
} printable_object_t;

typedef struct command_execute_object {
    bool success;
    uint8_t _0[3];
    printable_object_t printable_object;
} command_execute_object_t;

_Static_assert(sizeof(command_execute_object_t) == 28, "");

extern command_execute_object_t* lua_command_execute(command_execute_object_t* this, UNUSED int* param_2) {
    // prevent a crash
    this->printable_object.data[5] = 0xf;
    this->printable_object.data[4] = 0;
    *(uint8_t*)&this->printable_object.data[0] = 0;
    this->success = true;

    const char fmt[] = "0x%p, 0x%p";
    char msg[128];
    size_t num_msg_chars = sprintf(msg, fmt, this, param_2);

    print_to_object(&this->printable_object, msg, num_msg_chars);

    return this;
}