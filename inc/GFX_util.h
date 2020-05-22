#ifndef GFX_UTIL_H
#define GFX_UTIL_H

#include <stdint.h>

// VERA Memory map
// 0x0000-0x0FFF background tilemap (64 x 32 x 2 = 4096 bytes)
// 0x1000-0x1FFF foreground tilemap (64 x 32 x 2 = 4096 bytes)
// 0x2000-0x9FFF background tiles (1024 tiles * 32 bytes per tile = 32768 bytes)
// 0xA000-0x11FFF background tiles (1024 tiles * 32 bytes per tile = 32768 bytes)
// 0x12000-0x1F9BF unused

// Writing to foreground works, background does not
/*#define GFX_BACKGROUND_MAP_BASE_ADDR 0x0
#define GFX_FOREGROUND_MAP_BASE_ADDR 0x1000
#define GFX_BACKGROUND_TILE_BASE_ADDR 0x2000
#define GFX_FOREGROUND_TILE_BASE_ADDR 0xA000*/

#define GFX_BACKGROUND_MAP_BASE_ADDR 0x1000
#define GFX_FOREGROUND_MAP_BASE_ADDR 0x0
#define GFX_BACKGROUND_TILE_BASE_ADDR 0xA000
#define GFX_FOREGROUND_TILE_BASE_ADDR 0x2000

#define GFX_PALETTE_ADDR 0x1FA00

#define GFX_PLANE_WIDTH 64
#define GFX_PLANE_WIDTH_TIMES_TWO 128
#define GFX_PLANE_HEIGHT 32

void GFX_setBackgroundTile
    (
    uint8_t x,
    uint8_t y,
    uint16_t tile
    );

void GFX_setForegroundTile
    (
    uint8_t x,
    uint8_t y,
    uint16_t tile
    );

#endif
