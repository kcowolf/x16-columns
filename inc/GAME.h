#ifndef GAME_H
#define GAME_H

#define GAME_BOARD_WIDTH 6
#define GAME_BOARD_HEIGHT 15
#define GAME_FIRST_VISIBLE_ROW 2
#define GAME_FALLING_GEMS_COUNT 3

typedef enum
{
    GAME_ORIGINAL,
    GAME_FLASH
} GAME_Mode;

typedef enum
{
    GAME_FLASH_EASY,
    GAME_FLASH_NORMAL,
    GAME_FLASH_HARD
} GAME_FlashDifficulty;

typedef enum
{
    GAME_INIT,               // SCRN will transition after drawing the board.
    GAME_WAIT_FOR_START,
    GAME_INIT_GEMS,
    GAME_GEMS_INITIALIZED,   // Signal that SCRN should draw next gems.
    GAME_GEMS_FALLING,
    GAME_CHECK_FOR_MATCHES,
    GAME_WAIT_FOR_MATCHES,   // SCRN will transition after animating the matches.
    GAME_CLEAR_MATCHES,
    GAME_MATCHES_CLEARED,
    GAME_GAME_OVER,
    GAME_WAIT_FOR_GAME_OVER, // SCRN will transition after animating the game over.
    GAME_HALT
} GAME_State;

void GAME_init_original();

void GAME_init_flash(GAME_FlashDifficulty difficulty, uint8_t height);

void GAME_update();

extern uint8_t GAME_board[GAME_BOARD_WIDTH][GAME_BOARD_HEIGHT];
extern uint8_t GAME_matches[GAME_BOARD_WIDTH][GAME_BOARD_HEIGHT];

extern GAME_Mode GAME_mode;
extern GAME_State GAME_state;

extern uint8_t GAME_currentGems[GAME_FALLING_GEMS_COUNT];
extern uint8_t GAME_nextGems[GAME_FALLING_GEMS_COUNT];

extern uint8_t GAME_level;

extern uint8_t GAME_gemsFallTimer;
extern uint8_t GAME_gemsFallTimerHalf;

extern uint8_t GAME_gemX;
extern uint8_t GAME_gemY;  // Y coordinate (board row) of bottom gem.

#endif
