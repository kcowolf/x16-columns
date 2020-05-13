#define GFX_PLANE_WIDTH 64
#define GFX_PLANE_WIDTH_TIMES_TWO 128

#include <cx16.h>
#include "GFX_util.h"

void GFX_setTile
    (
    uint8_t x,
    uint8_t y,
    uint8_t tile
    )
{
    vpoke(tile, y * GFX_PLANE_WIDTH_TIMES_TWO + x);
}
