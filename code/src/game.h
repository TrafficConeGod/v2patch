#pragma once
#include "util.h"
#include <stdint.h>
#include <stdarg.h>

#define PLEASE_TYPE_SOME_LUA_CODE_MSG ((const char*)0xdf1454)

typedef void* THISCALL (*print_to_object_t) (void*, const char*, uint32_t);
static const print_to_object_t print_to_object = (print_to_object_t)0x409350;

typedef int CDECL (*sprintf_t) (char*, const char*, ...);
static const sprintf_t sprintf = (sprintf_t)0xaaee35;