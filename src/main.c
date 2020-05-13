#include <cx16.h>
#include <stdio.h>
#include <stdint.h>
#include <cbm.h>
#include <peekpoke.h>
#include "load.h"
#include "vload.h"

#include "GFX.h"

#define TILEMAP_WIDTH 40
#define TILEMAP_WIDTH_TIMES_TWO 80
#define PLANE_WIDTH 64
#define PLANE_WIDTH_TIMES_TWO 128

void colorTest();
void setTile(uint8_t x, uint8_t y, uint8_t tile);

int main()
{
    int result = 1;
	uint8_t x = 0;
    uint8_t y = 0;
	uint8_t* tilemap = (uint8_t*)0xa000;

    // Load palette
    result = vload_host(PALBG1_FILENAME, 0x1fa00);
    if (!result)
	{
		printf("  failed to load palette\n");
		return 1;
	}

	// Load tileset
    result = vload_host(TSBG1_FILENAME, 0x10000);
    if (!result)
	{
        printf("  failed to load tiles\n");
        return 1;
    }

	// Load splash tilemap to bank 21
    result = load_bank_host(TMBG1_FILENAME, 21);
    if (!result) 
	{
        printf("  failed to load splash tilemap\n");
        return 1;
	}

	// Initialize VERA
	VERA.control = 0;
    VERA.display.video |= 0x20;   // Layer 1 enabled, Layer 0 disabled, Sprites disabled

    VERA.display.hscale = 128;    // HSCALE=1
    VERA.display.vscale = 128;    // VSCALE=1
    VERA.layer0.config = 0x0;     // Disable Layer 0

    VERA.layer1.config = 0x12;    // 128x128 map, color depth 4bpp
    VERA.layer1.mapbase = 0x0;    // Map base at 0x00000
    VERA.layer1.tilebase = 0x80;  // Tile base at 0x10000, 8x8 tiles

    videomode(VIDEOMODE_40x30);

	// Write tilemap to VRAM
    VIA1.pra = 21;

    for (y = 0; y < TMBG1_TILE_HEIGHT; y++)
	{
        for (x = 0; x < TILEMAP_WIDTH_TIMES_TWO; x++)  // TIMES_TWO because we write two bytes per tile
		{
            uint8_t tile_value = tilemap[(y * TILEMAP_WIDTH_TIMES_TWO) + x];
			setTile(x, y, tile_value);
        }
	}

	return 0;
}

/*void colorTest()
{
	uint8_t i;

	for (i = 0; i < 16; i++)
	{
		uint8_t color = i << 4 | i;
		POKE(0x376, color);
		printf("    ");
	}
	
	POKE(0x376, 6 << 4 | 1);
}*/

void setTile(uint8_t x, uint8_t y, uint8_t tile)
{
    vpoke(tile, y * PLANE_WIDTH_TIMES_TWO + x);
}
