// Based on https://github.com/visrealm/supaplex-x16/blob/37e9a50ef796b1e7abb0bb5156cd1fd15106f84d/common/vera/constants.asm

// Commander X16
//
// Vera constants
//
// Copyright (c) 2020 Troy Schrapel
//
// This code is licensed under the MIT license
//
// https://github.com/visrealm/supaplex-x16
//

#define VERA_ADDRx_L 0x9F20
#define VERA_ADDRx_M 0x9F21
#define VERA_ADDRx_H 0x9F22
#define VERA_DATA0 0x9F23
#define VERA_DATA1 0x9F24
#define VERA_CTRL 0x9F25
#define VERA_IEN 0x9F26
#define VERA_ISR 0x9F27
#define VERA_IRQLINE_L 0x9F28
#define VERA_DC_VIDEO 0x9F29
#define VERA_DC_HSCALE 0x9F2A
#define VERA_DC_VSCALE 0x9F2B
#define VERA_DC_BORDER 0x9F2C
#define VERA_DC_HSTART 0x9F29
#define VERA_DC_HSTOP 0x9F2A
#define VERA_DC_VSTART 0x9F2B
#define VERA_DC_VSTOP 0x9F2C
#define VERA_L0_CONFIG 0x9F2D
#define VERA_L0_MAPBASE 0x9F2E
#define VERA_L0_TILEBASE 0x9F2F
#define VERA_L0_HSCROLL_L 0x9F30
#define VERA_L0_HSCROLL_H 0x9F31
#define VERA_L0_VSCROLL_L 0x9F32
#define VERA_L0_VSCROLL_H 0x9F33
#define VERA_L1_CONFIG 0x9F34
#define VERA_L1_MAPBASE 0x9F35
#define VERA_L1_TILEBASE 0x9F36
#define VERA_L1_HSCROLL_L 0x9F37
#define VERA_L1_HSCROLL_H 0x9F38
#define VERA_L1_VSCROLL_L 0x9F39
#define VERA_L1_VSCROLL_H 0x9F3A
#define VERA_AUDIO_CTRL 0x9F3B
#define VERA_AUDIO_RATE 0x9F3C
#define VERA_AUDIO_DATA 0x9F3D
#define VERA_SPI_DATA 0x9F3E
#define VERA_SPI_CTRL 0x9F3F

// ADDRx_H increments
// -----------------------------------------------------------------------------
#define VERA_INCR_0   0x00
#define VERA_INCR_1   0x10
#define VERA_INCR_2   0x20
#define VERA_INCR_4   0x30
#define VERA_INCR_8   0x40
#define VERA_INCR_16  0x50
#define VERA_INCR_32  0x60
#define VERA_INCR_64  0x70
#define VERA_INCR_128 0x80
#define VERA_INCR_256 0x90
#define VERA_INCR_512 0xA0
#define VERA_INCR_40  0xB0
#define VERA_INCR_80  0xC0
#define VERA_INCR_160 0xD0
#define VERA_INCR_320 0xE0
#define VERA_INCR_640 0xF0

#define VERA_DECR     0x08


// DC_VIDEO
// -----------------------------------------------------------------------------
#define VERA_VIDEO_SPRITES_ENABLED  0x40
#define VERA_VIDEO_LAYER1_ENABLED   0x20
#define VERA_VIDEO_LAYER0_ENABLED   0x10
#define VERA_VIDEO_OUTPUT_DISABLED  0x00
#define VERA_VIDEO_OUTPUT_VGA       0x01
#define VERA_VIDEO_OUTPUT_NTSC      0x02
#define VERA_VIDEO_OUTPUT_RGB       0x03

// DC_SCALE
// -----------------------------------------------------------------------------
#define VERA_SCALE_1x               0x80
#define VERA_SCALE_2x               0x40
#define VERA_SCALE_4x               0x20

// Lx_CONFIG
// -----------------------------------------------------------------------------
#define VERA_CONFIG_1BPP          0x0
#define VERA_CONFIG_2BPP          0x1
#define VERA_CONFIG_4BPP          0x2
#define VERA_CONFIG_8BPP          0x3

#define VERA_CONFIG_BITMAP_MODE   0x4

#define VERA_CONFIG_MAP_WIDTH_32  (0x0 << 4)
#define VERA_CONFIG_MAP_WIDTH_64  (0x1 << 4)
#define VERA_CONFIG_MAP_WIDTH_128 (0x2 << 4)
#define VERA_CONFIG_MAP_WIDTH_256 (0x3 << 4)

#define VERA_CONFIG_MAP_HEIGHT_32  (0x0 << 6)
#define VERA_CONFIG_MAP_HEIGHT_64  (0x1 << 6)
#define VERA_CONFIG_MAP_HEIGHT_128 (0x2 << 6)
#define VERA_CONFIG_MAP_HEIGHT_256 (0x3 << 6)

// Lx_TILEBASE
// -----------------------------------------------------------------------------
#define VERA_TILE_WIDTH_8   0x00
#define VERA_TILE_WIDTH_16  0x01
#define VERA_TILE_HEIGHT_8  0x00
#define VERA_TILE_HEIGHT_16 0x02

// Other Vera addresses
// -----------------------------------------------------------------------------
#define VERA_PALETTE 0x1FA00