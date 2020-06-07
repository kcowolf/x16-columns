#include <cbm.h>
#include <cx16.h>
#include <joystick.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "load.h"
#include "vload.h"

#include "GAME.h"
#include "GFX.h"
#include "GFX_util.h"
#include "GFX_vera.h"
#include "INPT.h"
#include "SCRN.h"

void initVera();

int main()
{
    srand(time(NULL));
    joy_install(cx16_std_joy);

    initVera();

    INPT_init();
    GAME_init(GAME_ORIGINAL);
    if (SCRN_init() == false)
    {
        // Failed to load resources.
        return 1;
    }

    while(1)
    {
        INPT_update();
        GAME_update();
        SCRN_update();

        waitvsync();
    }

    return 0;
}

void initVera()
{
    uint8_t i = 0;
    uint8_t j = 0;

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

    VERA.display.video = VERA_VIDEO_LAYER1_ENABLED | VERA_VIDEO_SPRITES_ENABLED | VERA_VIDEO_OUTPUT_VGA;

    videomode(VIDEOMODE_40x30);

    for (i = 0; i < GFX_LAYER_WIDTH; i++)
    {
        for (j = 0; j < GFX_LAYER_HEIGHT; j++)
        {
            //GFX_setBackgroundTile(i, j, 0);
            GFX_setForegroundTile(i, j, 0);
        }
    }

    for (i = 0; i < VERA_SPRITE_COUNT; i++)
    {
        GFX_setSpriteDepth(i, VERA_SPRITE_DISABLED);
    }
}
