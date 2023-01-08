#pragma once

#define SIZEOF_ARRAY(arr) sizeof(arr)/sizeof(arr[0])
#define M_TAU (GLM_PI * 2)
#define VEC2(x, y) (vec2s){{x, y}}
#define VEC3(x, y, z) (vec3s){{x, y, z}}
#define PACKED __attribute__((__packed__))
#define UNUSED __attribute__((__unused__))