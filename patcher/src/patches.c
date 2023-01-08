#include "patches.h"
#include "util.h"
#include <string.h>
#include <stdint.h>

#define CREATING_CHECKSUM_MSG 0xa061b8

void apply_patches(UNUSED size_t size, void* data_v) {
    uint8_t* data = data_v;

    const char confirm_msg[15] = "PATCH_WORKED";
    memcpy(&data[CREATING_CHECKSUM_MSG], confirm_msg, sizeof(confirm_msg));

    data[0x1ceb0] = 0xc3;
}