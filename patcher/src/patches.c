#include "patches.h"
#include "util.h"
#include <string.h>
#include <stdint.h>

void apply_patches(UNUSED size_t size, void* data_v) {
    uint8_t* data = data_v;
    const char msg[15] = "CREATING_SHASUM";
    memcpy(&data[0xa061b8], msg, sizeof(msg));
}