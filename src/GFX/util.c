#define GFX_PLANE_WIDTH 64
#define GFX_PLANE_WIDTH_TIMES_TWO 128

#include <cx16.h>
#include "GFX_util.h"

void GFX_setBackgroundTile
    (
    uint8_t x,
    uint8_t y,
    uint16_t tile
    )
{
    vpoke(tile >> 8, (y * GFX_PLANE_WIDTH_TIMES_TWO) + (2 * x) + 1 + GFX_BACKGROUND_MAP_BASE_ADDR);
    vpoke(tile & 0xFF, (y * GFX_PLANE_WIDTH_TIMES_TWO) + (2 * x) + GFX_BACKGROUND_MAP_BASE_ADDR);
}

void GFX_setForegroundTile
    (
    uint8_t x,
    uint8_t y,
    uint16_t tile
    )
{
    vpoke(tile >> 8, (y * GFX_PLANE_WIDTH_TIMES_TWO) + (2 * x) + 1 + GFX_FOREGROUND_MAP_BASE_ADDR);
    vpoke(tile & 0xFF, (y * GFX_PLANE_WIDTH_TIMES_TWO) + (2 * x) + GFX_FOREGROUND_MAP_BASE_ADDR);
}
