#include <cx16.h>
#include <load.h>
#include <stdio.h>
#include <vload.h>
#include "GAME.h"
#include "GFX.h"
#include "GFX_util.h"
#include "GFX_vera.h"
#include "SCRN.h"
#include "TEXT.h"

#define GEMS_COUNT 8
#define EMPTYGEM_TILEMAP 0xA000
#define GEM1_TILEMAP 0xA008
#define GEM2_TILEMAP 0xA010
#define GEM3_TILEMAP 0xA018
#define GEM4_TILEMAP 0xA020
#define GEM5_TILEMAP 0xA028
#define GEM6_TILEMAP 0xA030
#define MAGICGEM_TILEMAP 0xA038

#define EXPLODE_TILEMAPS_COUNT 9
#define EXPLODE0_TILEMAP 0xA040
#define EXPLODE1_TILEMAP 0xA048
#define EXPLODE2_TILEMAP 0xA050
#define EXPLODE3_TILEMAP 0xA058
#define EXPLODE4_TILEMAP 0xA060
#define EXPLODE5_TILEMAP 0xA068
#define EXPLODE6_TILEMAP 0xA070
#define EXPLODE7_TILEMAP 0xA078

#define BOARD_VISIBLE_ROWS 13

#define BOARD_TILE_X 2
#define BOARD_TILE_Y 2
#define BOARD_PIXEL_X 16
#define BOARD_PIXEL_Y 16

#define NEXT_TILE_X 15
#define NEXT_TILE_Y 2

#define GEMS_MBANK 21

#define GEM_PIXEL_W 16
#define GEM_PIXEL_H 16

#define SPRITE_16x16_BYTES 128

#define MATCH_BLINK_FRAMES 16
#define MATCH_EXPLODE_FRAMES 18
#define MATCH_TOTAL_FRAMES 34

#define MAGIC_PALETTE_INDEX 5
#define MAGIC_PALETTE_COUNT 6
#define MAGIC_PALETTE_INITIAL_FRAMES 6

#define EXPLODE_ALL_FRAMES_PER_ROW 2

// x, y coordinates relative to the board
#define FLASH_GEM_X 3
#define FLASH_GEM_Y 14

static void animateExplodeAll();
static void animateMatches();
static void clearFallingGems();
static void clearNextGems();
static void drawFallingGems();
static void drawGem
    (
    uint8_t tileX,
    uint8_t tileY,
    const uint16_t* gemTileAddr
    );
static void drawNextGems();
static void redrawBoard();
static void redrawFlashGem();
static void updateMagicPalette();

static uint8_t animationFrames;
static uint8_t currentMagicPalette;
static uint8_t magicPaletteFrames;
static uint8_t explodeAllCurrentRow;
static uint16_t gameTimer;

static const uint16_t* GEM_TILEMAPS[GEMS_COUNT] =
{
    (const uint16_t*)EMPTYGEM_TILEMAP,
    (const uint16_t*)GEM1_TILEMAP,
    (const uint16_t*)GEM2_TILEMAP,
    (const uint16_t*)GEM3_TILEMAP,
    (const uint16_t*)GEM4_TILEMAP,
    (const uint16_t*)GEM5_TILEMAP,
    (const uint16_t*)GEM6_TILEMAP,
    (const uint16_t*)MAGICGEM_TILEMAP
};

static const uint16_t* EXPLODE_TILEMAPS[EXPLODE_TILEMAPS_COUNT] =
{
    (const uint16_t*)EMPTYGEM_TILEMAP,
    (const uint16_t*)EXPLODE7_TILEMAP,
    (const uint16_t*)EXPLODE6_TILEMAP,
    (const uint16_t*)EXPLODE5_TILEMAP,
    (const uint16_t*)EXPLODE4_TILEMAP,
    (const uint16_t*)EXPLODE3_TILEMAP,
    (const uint16_t*)EXPLODE2_TILEMAP,
    (const uint16_t*)EXPLODE1_TILEMAP,
    (const uint16_t*)EXPLODE0_TILEMAP,
};

static const uint16_t GEM_SPRITE_BYTE_OFFSETS[GEMS_COUNT] =
{
    (0 * SPRITE_16x16_BYTES), // Empty
    (1 * SPRITE_16x16_BYTES),
    (2 * SPRITE_16x16_BYTES),
    (3 * SPRITE_16x16_BYTES),
    (4 * SPRITE_16x16_BYTES),
    (5 * SPRITE_16x16_BYTES),
    (6 * SPRITE_16x16_BYTES),
    (7 * SPRITE_16x16_BYTES), // Magic gem
};

static const uint8_t GEM_PALETTES[GEMS_COUNT] =
{
    3,
    3,
    3,
    3,
    4,
    4,
    4,
    MAGIC_PALETTE_INDEX
};

static const uint16_t MAGIC_COLORS[MAGIC_PALETTE_COUNT][3] =
{
    {
        0x0C01,
        0x0E00,
        0x0F75
    },
    {
        0x0EA0,
        0x0FB1,
        0x0FFA
    },
    {
        0x044F,
        0x086F,
        0x0ADF
    },
    {
        0x0088,
        0x00CC,
        0x05FF
    },
    {
        0x0B0C,
        0x0E0E,
        0x0FCF
    },
    {
        0x00A1,
        0x00C4,
        0x06F5
    }
};

bool SCRN_init()
{
    int result;

    // Load palette
    result = vload_host(PAL2_FILENAME, GFX_PALETTE_ADDR);
    if (!result)
    {
        printf("  failed to load palette\n");
        return false;
    }

    // Load background tileset
    // TODO -- Split tileset into foreground/background.  TS1 has too many tiles to fit in background tile area.
    /*result = vload_host(, GFX_BACKGROUND_TILE_BASE_ADDR);
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

    // Load game background tilemap to VRAM
    result = vload_host(TMBG2_FILENAME, GFX_FOREGROUND_MAP_BASE_ADDR);
    if (!result)
    {
        printf("  failed to load bg tilemap\n");
        return false;
    }

    VIA1.pra = GEMS_MBANK;
    result = result && load_file_host(TMGEMS0_FILENAME, EMPTYGEM_TILEMAP);
    result = result && load_file_host(TMGEMS1_FILENAME, GEM1_TILEMAP);
    result = result && load_file_host(TMGEMS2_FILENAME, GEM2_TILEMAP);
    result = result && load_file_host(TMGEMS3_FILENAME, GEM3_TILEMAP);
    result = result && load_file_host(TMGEMS4_FILENAME, GEM4_TILEMAP);
    result = result && load_file_host(TMGEMS5_FILENAME, GEM5_TILEMAP);
    result = result && load_file_host(TMGEMS6_FILENAME, GEM6_TILEMAP);
    result = result && load_file_host(TMGEMSM_FILENAME, MAGICGEM_TILEMAP);
    result = result && load_file_host(TMEXPL0_FILENAME, EXPLODE0_TILEMAP);
    result = result && load_file_host(TMEXPL1_FILENAME, EXPLODE1_TILEMAP);
    result = result && load_file_host(TMEXPL2_FILENAME, EXPLODE2_TILEMAP);
    result = result && load_file_host(TMEXPL3_FILENAME, EXPLODE3_TILEMAP);
    result = result && load_file_host(TMEXPL4_FILENAME, EXPLODE4_TILEMAP);
    result = result && load_file_host(TMEXPL5_FILENAME, EXPLODE5_TILEMAP);
    result = result && load_file_host(TMEXPL6_FILENAME, EXPLODE6_TILEMAP);
    result = result && load_file_host(TMEXPL7_FILENAME, EXPLODE7_TILEMAP);
    if (!result)
    {
        printf("  failed to load gems\n");
        return false;
    }

    result = vload_host(SPR1_FILENAME, GFX_SPRITE_BASE_ADDR);
    if (!result)
    {
        printf("  failed to load sprites\n");
        return false;
    }

    currentMagicPalette = 0;
    magicPaletteFrames = MAGIC_PALETTE_INITIAL_FRAMES;

    gameTimer = 0;

    return true;
}

void SCRN_update()
{
    updateMagicPalette();
    
    gameTimer++;

    switch (GAME_state)
    {
        case GAME_INIT:
            redrawBoard();
            GAME_state = GAME_WAIT_FOR_START;
            break;
        case GAME_GEMS_INITIALIZED:
            drawNextGems();
            break;
        case GAME_GEMS_FALLING:
            drawFallingGems();
            redrawFlashGem();
            break;
        case GAME_CHECK_FOR_MATCHES:
            redrawBoard();
            clearFallingGems();
            animationFrames = MATCH_TOTAL_FRAMES;
            break;
        case GAME_WAIT_FOR_MATCHES:
            animateMatches();
            if (animationFrames == 0)
            {
                GAME_state = GAME_CLEAR_MATCHES;
            }
            redrawFlashGem();
            break;
        case GAME_MATCHES_CLEARED:
            redrawBoard();
            break;
        case GAME_GAME_OVER:
            animationFrames = 1;
            explodeAllCurrentRow = BOARD_VISIBLE_ROWS + EXPLODE_TILEMAPS_COUNT;
            clearNextGems();
            break;
        case GAME_WAIT_FOR_GAME_OVER:
            animateExplodeAll();
            break;
        default:
            break;
    }
}

static void animateExplodeAll()
{
    uint8_t i;
    uint8_t boardX;
    uint8_t boardY; // Relative to the visible board (doesn't include the invisible rows at the top).
    const uint16_t* gemTileAddr;

    animationFrames--;

    if (animationFrames == 0)
    {
        animationFrames = EXPLODE_ALL_FRAMES_PER_ROW;

        if (explodeAllCurrentRow == 0)
        {
            GFX_drawForegroundText(TEXT_GAME_OVER, BOARD_TILE_X, 12);
            GFX_drawForegroundText(TEXT_PRESS_FIRE_BUTTON_1, BOARD_TILE_X, 14);
            GFX_drawForegroundText(TEXT_PRESS_FIRE_BUTTON_2, BOARD_TILE_X, 15);
            GAME_state = GAME_HALT;
        }
        else
        {
            explodeAllCurrentRow--;

            boardY = explodeAllCurrentRow;

            for (i = 0; i < EXPLODE_TILEMAPS_COUNT; i++)
            {
                if (boardY < BOARD_VISIBLE_ROWS)
                {
                    gemTileAddr = EXPLODE_TILEMAPS[i];

                    for (boardX = 0; boardX < GAME_BOARD_WIDTH; boardX++)
                    {
                        drawGem((boardX * 2) + BOARD_TILE_X, (boardY * 2) + BOARD_TILE_Y, gemTileAddr);
                    }
                }

                if (boardY == 0)
                {
                    // Break out of the for-loop.  We've reached the top of the board.
                    break;
                }

                boardY--;
            }
        }
    }
}

static void animateMatches()
{
    uint8_t x;
    uint8_t y;
    const uint16_t* gemTileAddr;

    animationFrames--;

    for (x = 0; x < GAME_BOARD_WIDTH; x++)
    {
        for (y = 0; y < BOARD_VISIBLE_ROWS; y++)
        {
            if (GAME_matches[x][y + GAME_FIRST_VISIBLE_ROW] == 1)
            {
                if (animationFrames >= MATCH_EXPLODE_FRAMES && (animationFrames / 2) & 1)
                {
                    gemTileAddr = GEM_TILEMAPS[0];
                }
                else if (animationFrames >= MATCH_EXPLODE_FRAMES)
                {
                    gemTileAddr = GEM_TILEMAPS[GAME_board[x][y + GAME_FIRST_VISIBLE_ROW]];
                }
                else
                {
                    gemTileAddr = EXPLODE_TILEMAPS[animationFrames / 2];
                }

                drawGem((x * 2) + BOARD_TILE_X, (y * 2) + BOARD_TILE_Y, gemTileAddr);
            }
        }
    }
}

static void clearFallingGems()
{
    int8_t i;

    for(i = 2; i >= 0; i--)
    {
        GFX_setSpriteDepth(i, VERA_SPRITE_DISABLED);
    }
}

static void clearNextGems()
{
    int8_t i;

    for (i = 2; i >= 0; i--)
    {
        drawGem(NEXT_TILE_X, (i * 2) + NEXT_TILE_Y, EMPTYGEM_TILEMAP);
    }
}

static void drawFallingGems()
{
    int8_t i;
    uint16_t x;
    uint16_t y;

    x = BOARD_PIXEL_X + (GAME_gemX * GEM_PIXEL_W);
    y = BOARD_PIXEL_X + ((GAME_gemY - GAME_FIRST_VISIBLE_ROW) * GEM_PIXEL_W);

    for(i = 2; i >= 0; i--)
    {
        if(y >= BOARD_PIXEL_Y)
        {
            GFX_setSprite(i, GFX_SPRITE_BASE_ADDR + GEM_SPRITE_BYTE_OFFSETS[GAME_currentGems[i]], x, y, VERA_SPRITE_ABOVE_L1, VERA_SPRITE_WIDTH_16 | VERA_SPRITE_HEIGHT_16 | GEM_PALETTES[GAME_currentGems[i]]);
        }
        else if(y + (GEM_PIXEL_H / 2) >= BOARD_PIXEL_Y)
        {
            // draw half sprite
        }
        else
        {
            GFX_setSpriteDepth(i, VERA_SPRITE_DISABLED);
        }
        y -= GEM_PIXEL_H;
    }
}

static void drawGem
    (
    uint8_t tileX,
    uint8_t tileY,
    const uint16_t* gemTileAddr
    )
{
    GFX_setForegroundTile(tileX, tileY, *gemTileAddr);
    gemTileAddr++;
    GFX_setForegroundTile(tileX + 1, tileY, *gemTileAddr);
    gemTileAddr++;
    GFX_setForegroundTile(tileX, tileY + 1, *gemTileAddr);
    gemTileAddr++;
    GFX_setForegroundTile(tileX + 1, tileY + 1, *gemTileAddr);
}

static void drawNextGems()
{
    int8_t i;

    for (i = 2; i >= 0; i--)
    {
        drawGem(NEXT_TILE_X, (i * 2) + NEXT_TILE_Y, GEM_TILEMAPS[GAME_nextGems[i]]);
    }
}

static void redrawBoard()
{
    uint8_t x;
    uint8_t y;

    for (x = 0; x < GAME_BOARD_WIDTH; x++)
    {
        for (y = 0; y < BOARD_VISIBLE_ROWS; y++)
        {
            if (x == FLASH_GEM_X && y + GAME_FIRST_VISIBLE_ROW == FLASH_GEM_Y)
            {
                redrawFlashGem();
            }
            else
            {
                drawGem((x * 2) + BOARD_TILE_X, (y * 2) + BOARD_TILE_Y, GEM_TILEMAPS[GAME_board[x][y + GAME_FIRST_VISIBLE_ROW]]);
            }
        }
    }
}

static void redrawFlashGem()
{
    // If this gem is part of a match, it is being animated.
    if (GAME_state == GAME_WAIT_FOR_MATCHES && GAME_matches[FLASH_GEM_X][FLASH_GEM_Y] == 1)
    {
        return;
    }

    if (GAME_mode == GAME_FLASH && (gameTimer & 0x40))
    {
        drawGem((FLASH_GEM_X * 2) + BOARD_TILE_X, ((FLASH_GEM_Y - GAME_FIRST_VISIBLE_ROW) * 2) + BOARD_TILE_Y, EMPTYGEM_TILEMAP);
    }
    else
    {
        drawGem((FLASH_GEM_X * 2) + BOARD_TILE_X, ((FLASH_GEM_Y - GAME_FIRST_VISIBLE_ROW) * 2) + BOARD_TILE_Y, GEM_TILEMAPS[GAME_board[FLASH_GEM_X][FLASH_GEM_Y]]);
    }
}

static void updateMagicPalette()
{
    magicPaletteFrames--;
    if (magicPaletteFrames == 0)
    {
        magicPaletteFrames = MAGIC_PALETTE_INITIAL_FRAMES;

        currentMagicPalette++;
        if (currentMagicPalette == MAGIC_PALETTE_COUNT)
        {
            currentMagicPalette = 0;
        }

        GFX_setColor(MAGIC_PALETTE_INDEX, 2, MAGIC_COLORS[currentMagicPalette][0]);
        GFX_setColor(MAGIC_PALETTE_INDEX, 3, MAGIC_COLORS[currentMagicPalette][1]);
        GFX_setColor(MAGIC_PALETTE_INDEX, 4, MAGIC_COLORS[currentMagicPalette][2]);
    }
}
