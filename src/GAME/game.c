#include <cx16.h>
#include <stdint.h>
#include "GAME.h"
#include "GFX.h"
#include "GFX_util.h"

#define BKGD_TILEMAP_WIDTH 40
#define BKGD_TILEMAP_WIDTH_TIMES_TWO 80

typedef enum
{
    STATE_DROP_GEMS,
    STATE_CHECK_MATCHES,
    STATE_GAME_OVER
} GameState;

static GameState gameState;

void GAME_init()
{
	uint8_t x = 0;
    uint8_t y = 0;
    uint8_t* tilemap = (uint8_t*)0xa000;

    // Write tilemap to VRAM
    VIA1.pra = GAME_BKG1_MBANK;

    for (y = 0; y < TMBG1_TILE_HEIGHT; y++)
	{
        for (x = 0; x < BKGD_TILEMAP_WIDTH_TIMES_TWO; x++)  // TIMES_TWO because we write two bytes per tile
		{
            uint8_t tile_value = tilemap[(y * BKGD_TILEMAP_WIDTH_TIMES_TWO) + x];
			GFX_setTile(x, y, tile_value);
        }
	}


}