#pragma once

#include <stdint.h>

#define true 1
#define false 0

#define global_variable static
#define internal static

typedef int8_t s8;
typedef uint8_t u8;

typedef int16_t s16;
typedef uint16_t u16;

typedef int32_t s32;
typedef uint32_t u32;

typedef int64_t s64;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef int b32;


struct 
{
    // Platform non-specific part
    int width, height;
    u32* pixels;
    
    // Platform specific part
    BITMAPINFO bitmap_info;
} typedef RenderBuffer;

#define ArrayCount(a) (sizeof(a)/sizeof(a[0]))

#if DEVELOPMENT
#define assert(c) {if(!c) {*(int*) 0=0;} else{}}
#else
#define assert(c)
#endif 

#define invalid_default_case default: {assert(0);}