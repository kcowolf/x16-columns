#ifndef GFX_UTIL_H
#define GFX_UTIL_H

#include <stdint.h>

#define GFX_PLANE_WIDTH 64
#define GFX_PLANE_WIDTH_TIMES_TWO 128

void GFX_setTile
    (
    uint8_t x,
    uint8_t y,
    uint8_t tile
    );

#endif
