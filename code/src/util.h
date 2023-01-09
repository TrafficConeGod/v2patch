#pragma once

#define NUM_ELEMS(arr) sizeof(arr)/sizeof(arr[0])
#define PACKED __attribute__((__packed__))
#define UNUSED __attribute__((__unused__))
#define THISCALL __attribute__((__thiscall__))