#include <cx16.h>
#include <stdio.h>
#include <stdint.h>
#include <cbm.h>
#include "load.h"
#include "vload.h"

#include "GAME.h"
#include "GFX.h"

void initVera();
int loadResources();

int main()
{
    if (!loadResources())
    {
        return 1;
    }

    initVera();

    GAME_init();

	return 0;
}

void initVera()
{
	// Initialize VERA
	VERA.control = 0;
    VERA.display.video |= 0x20;   // Layer 1 enabled, Layer 0 disabled, Sprites disabled

    VERA.display.hscale = 128;    // HSCALE=1
    VERA.display.vscale = 128;    // VSCALE=1
    VERA.layer0.config = 0x0;     // Disable Layer 0

    VERA.layer1.config = 0x12;    // 64x32 map, color depth 4bpp
    VERA.layer1.mapbase = 0x0;    // Map base at 0x00000
    VERA.layer1.tilebase = 0x80;  // Tile base at 0x10000, 8x8 tiles

    videomode(VIDEOMODE_40x30);
}

int loadResources()
{
    int result;

    // Load palette
    result = vload_host(PALBG1_FILENAME, 0x1fa00);
    if (!result)
	{
		printf("  failed to load palette\n");
		return 0;
	}

	// Load tileset
    result = vload_host(TSBG1_FILENAME, 0x10000);
    if (!result)
	{
        printf("  failed to load tiles\n");
        return 0;
    }

	// Load splash tilemap to bank
    result = load_bank_host(TMBG1_FILENAME, GAME_BKG1_MBANK);
    if (!result) 
	{
        printf("  failed to load splash tilemap\n");
        return 0;
	}

    return 1;
}
