#include <cx16.h>
#include <stdbool.h>
#include <stdint.h>
#include <cbm.h>
#include "load.h"
#include "vload.h"

#include "GAME.h"
#include "GFX.h"
#include "GFX_util.h"
#include "GFX_vera.h"
#include "SCRN.h"

void initVera();

int main()
{
    initVera();

    GAME_init();
    if (SCRN_init() == false)
    {
        // Failed to load resources.
        return 1;
    }

    while(1)
    {
        GAME_update();
        SCRN_update();
    }

    return 0;
}

void initVera()
{
    uint8_t x = 0;
    uint8_t y = 0;

    // Initialize VERA
    VERA.control = 0;

    // Initialize VERA
    VERA.display.hscale = VERA_SCALE_1x;
    VERA.display.vscale = VERA_SCALE_1x;

    VERA.layer0.config = VERA_CONFIG_MAP_WIDTH_64 | VERA_CONFIG_MAP_HEIGHT_32 | VERA_CONFIG_4BPP;
    VERA.layer0.mapbase = GFX_BACKGROUND_MAP_BASE_ADDR >> 9;
    VERA.layer0.tilebase = GFX_BACKGROUND_TILE_BASE_ADDR >> 9 | VERA_TILE_HEIGHT_8 | VERA_TILE_WIDTH_8;
    VERA.layer0.hscroll = 0;
    VERA.layer0.vscroll = 0;

    VERA.layer1.config = VERA_CONFIG_MAP_WIDTH_64 | VERA_CONFIG_MAP_HEIGHT_32 | VERA_CONFIG_4BPP;
    VERA.layer1.mapbase = GFX_FOREGROUND_MAP_BASE_ADDR >> 9;
    VERA.layer1.tilebase = GFX_FOREGROUND_TILE_BASE_ADDR >> 9 | VERA_TILE_HEIGHT_8 | VERA_TILE_WIDTH_8;
    VERA.layer1.hscroll = 0;
    VERA.layer1.vscroll = 0;

    VERA.display.video = VERA_VIDEO_LAYER1_ENABLED | VERA_VIDEO_OUTPUT_VGA;

    videomode(VIDEOMODE_40x30);

    for (y = 0; y < GFX_PLANE_HEIGHT; y++)
    {
        for (x = 0; x < GFX_PLANE_WIDTH; x++)
        {
            //GFX_setBackgroundTile(x, y, 0);
            GFX_setForegroundTile(x, y, 0);
        }
    }
}
