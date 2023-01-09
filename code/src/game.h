#pragma once
#include "util.h"
#include <stdint.h>

#define PLEASE_TYPE_SOME_LUA_CODE_MSG ((const char*)0xdf1454)

inline void* print_to_debug_console(void* this, const char* msg, uint32_t size) {
    return ((void* THISCALL (*) (void*, const char*, uint32_t))0x409350)(this, msg, size);
}