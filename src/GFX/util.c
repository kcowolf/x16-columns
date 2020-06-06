#include <cx16.h>
#include "GFX_util.h"
#include "GFX_vera.h"

#define NULL_CHAR '\0'
#define FONT_FIRST_CHAR ' '

void GFX_drawForegroundText
    (
    const unsigned char* str,
    uint16_t x,
    uint16_t y
    )
{
    while ((*str) != NULL_CHAR)
    {
        GFX_setForegroundTile(x, y, (*str) - FONT_FIRST_CHAR);
        x++;
        str++;
    }
}

void GFX_drawForegroundTextN
    (
    const unsigned char* str,
    uint16_t x,
    uint16_t y,
    uint8_t count
    )
{
    while (count > 0)
    {
        GFX_setForegroundTile(x, y, (*str) - FONT_FIRST_CHAR);
        x++;
        str++;
        count--;
    }
}

void GFX_setBackgroundTile
    (
    uint8_t x,
    uint8_t y,
    uint16_t tile
    )
{
    vpoke(tile >> 8, (y * GFX_LAYER_WIDTH_TIMES_TWO) + (2 * x) + 1 + GFX_BACKGROUND_MAP_BASE_ADDR);
    vpoke(tile & 0xFF, (y * GFX_LAYER_WIDTH_TIMES_TWO) + (2 * x) + GFX_BACKGROUND_MAP_BASE_ADDR);
}

void GFX_setForegroundTile
    (
    uint8_t x,
    uint8_t y,
    uint16_t tile
    )
{
    vpoke(tile >> 8, (y * GFX_LAYER_WIDTH_TIMES_TWO) + (2 * x) + 1 + GFX_FOREGROUND_MAP_BASE_ADDR);
    vpoke(tile & 0xFF, (y * GFX_LAYER_WIDTH_TIMES_TWO) + (2 * x) + GFX_FOREGROUND_MAP_BASE_ADDR);
}

void GFX_setSprite
    (
    uint8_t i,
    uint16_t addr,
    uint16_t x,
    uint16_t y,
    uint8_t mask_depth_flip,
    uint8_t size_palette
    )
{
    uint32_t spriteAttrAddr = (GFX_SPRITE_ATTR_ADDR + (VERA_SPRITE_SIZE * i)) | ((uint32_t)VERA_INC_1 << 16);

    vpoke((addr >> 5) & 0xFF, spriteAttrAddr);
    VERA.data0 = (addr >> 13) & 0xF | VERA_SPRITE_MODE_4BPP;
    VERA.data0 = x & 0xFF;
    VERA.data0 = x >> 8;
    VERA.data0 = y & 0xFF;
    VERA.data0 = y >> 8;
    VERA.data0 = mask_depth_flip;
    VERA.data0 = size_palette;
}

void GFX_setSpriteDepth
    (
    uint8_t i,
    uint8_t depth
    )
{
    vpoke(depth, GFX_SPRITE_ATTR_ADDR + (VERA_SPRITE_SIZE * i) + 6);  // 8 bytes per sprite, depth is in byte 6
}

void GFX_setSpritePosition
    (
    uint8_t i,
    uint8_t x,
    uint8_t y
    )
{
    VERA.address_hi = VERA_INCR_1;
    vpoke(x & 0xFF, GFX_SPRITE_ATTR_ADDR + (VERA_SPRITE_SIZE * i) + 2);  // 8 bytes per sprite, x starts in byte 2
    VERA.data0 = x >> 8;
    VERA.data0 = y & 0xFF;
    VERA.data0 = y >> 8;
}
