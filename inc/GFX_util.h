#ifndef GFX_UTIL_H
#define GFX_UTIL_H

#include <stdint.h>

// VERA Memory map
// 0x0000-0x0FFF background tilemap (64 x 32 x 2 = 4096 bytes)
// 0x1000-0x1FFF foreground tilemap (64 x 32 x 2 = 4096 bytes)
// 0x2000-0x5FFF background tiles (512 tiles * 32 bytes per tile = 16384 bytes)
// 0x6000-0xDFFF foreground tiles (1024 tiles * 32 bytes per tile = 32768 bytes)
// 0xE000-0xFFFF sprites

// Writing to foreground works, background does not
#define GFX_BACKGROUND_MAP_BASE_ADDR 0x0
#define GFX_FOREGROUND_MAP_BASE_ADDR 0x1000
#define GFX_BACKGROUND_TILE_BASE_ADDR 0x2000
#define GFX_FOREGROUND_TILE_BASE_ADDR 0x6000
#define GFX_SPRITE_BASE_ADDR 0xE000

#define GFX_PALETTE_ADDR 0x1FA00
#define GFX_SPRITE_ATTR_ADDR 0x1FC00

#define GFX_LAYER_WIDTH 64
#define GFX_LAYER_WIDTH_TIMES_TWO 128
#define GFX_LAYER_HEIGHT 32

void GFX_drawForegroundText
    (
    const unsigned char* str,
    uint16_t x,
    uint16_t y
    );

void GFX_drawForegroundTextN
    (
    const unsigned char* str,
    uint16_t x,
    uint16_t y,
    uint8_t count
    );

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

void GFX_setSprite
    (
    uint8_t i,
    uint16_t addr, // Must be divisible by 32.
    uint16_t x,
    uint16_t y,
    uint8_t mask_depth_flip,
    uint8_t size_palette
    );

void GFX_setSpriteDepth
    (
    uint8_t i,
    uint8_t depth
    );

#endif
