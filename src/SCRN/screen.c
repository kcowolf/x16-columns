#include <cx16.h>
#include <load.h>
#include <stdio.h>
#include <vload.h>
#include "GAME.h"
#include "GFX.h"
#include "GFX_util.h"
#include "SCRN.h"

void redrawField();

#define BANK_RAM_START 0xA000

#define GEMS_COUNT 7
#define GEM0_TILES 0xA000
#define GEM1_TILES 0xA008
#define GEM2_TILES 0xA010
#define GEM3_TILES 0xA018
#define GEM4_TILES 0xA020
#define GEM5_TILES 0xA028
#define GEM6_TILES 0xA030

#define BOARD_VISIBLE_ROWS 13
#define FIRST_VISIBLE_ROW 3

#define GEMS_MBANK 21

#define BOARD_TILE_X 2
#define BOARD_TILE_Y 2

const uint16_t* gemTiles[GEMS_COUNT] =
{
    (const uint16_t*)GEM0_TILES,
    (const uint16_t*)GEM1_TILES,
    (const uint16_t*)GEM2_TILES,
    (const uint16_t*)GEM3_TILES,
    (const uint16_t*)GEM4_TILES,
    (const uint16_t*)GEM5_TILES,
    (const uint16_t*)GEM6_TILES,
};

bool SCRN_init()
{
    int result;

    // Load palette
    result = vload_host(PAL1_FILENAME, GFX_PALETTE_ADDR);
    if (!result)
    {
        printf("  failed to load palette\n");
        return false;
    }

    // Load background tileset
    /*result = vload_host(TS1_FILENAME, GFX_BACKGROUND_TILE_BASE_ADDR);
    if (!result)
    {
        printf("  failed to load bg tiles\n");
        return 0;
    }*/

    // Load foreground tileset
    result = vload_host(TS1_FILENAME, GFX_FOREGROUND_TILE_BASE_ADDR);
    if (!result)
    {
        printf("  failed to load fg tiles\n");
        return false;
    }

    // Load splash tilemap to VRAM
    result = vload_host(TMBG1_FILENAME, GFX_FOREGROUND_MAP_BASE_ADDR);
    if (!result)
    {
        printf("  failed to load bg tilemap\n");
        return false;
    }

    VIA1.pra = GEMS_MBANK;
    result = load_file_host(TMGEMS0_FILENAME, GEM0_TILES);
    result = result && load_file_host(TMGEMS1_FILENAME, GEM1_TILES);
    result = result && load_file_host(TMGEMS2_FILENAME, GEM2_TILES);
    result = result && load_file_host(TMGEMS3_FILENAME, GEM3_TILES);
    result = result && load_file_host(TMGEMS4_FILENAME, GEM4_TILES);
    result = result && load_file_host(TMGEMS5_FILENAME, GEM5_TILES);
    result = result && load_file_host(TMGEMS6_FILENAME, GEM6_TILES);
    if (!result)
    {
        printf("  failed to load gems\n");
        return false;
    }

    return true;
}

void SCRN_update()
{
    switch (GAME_state)
    {
        case GAME_INIT:
            redrawField();
            GAME_state = GAME_WAIT_FOR_START;
            break;
        case GAME_WAIT_FOR_START:
            break;
    }
}

void redrawField()
{
    uint8_t x;
    uint8_t y;

    for (x = 0; x < GAME_BOARD_WIDTH; x++)
    {
        for (y = 0; y < BOARD_VISIBLE_ROWS; y++)
        {
            const uint16_t* gemTileAddr = gemTiles[GAME_board[x][y + FIRST_VISIBLE_ROW]];
            GFX_setForegroundTile((x * 2) + BOARD_TILE_X, (y * 2) + BOARD_TILE_Y, *gemTileAddr);
            gemTileAddr++;
            GFX_setForegroundTile((x * 2) + BOARD_TILE_X + 1, (y * 2) + BOARD_TILE_Y, *gemTileAddr);
            gemTileAddr++;
            GFX_setForegroundTile((x * 2) + BOARD_TILE_X, (y * 2) + BOARD_TILE_Y + 1, *gemTileAddr);
            gemTileAddr++;
            GFX_setForegroundTile((x * 2) + BOARD_TILE_X + 1, (y * 2) + BOARD_TILE_Y + 1, *gemTileAddr);
        }
    }
}
