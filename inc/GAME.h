#ifndef GAME_H
#define GAME_H

#define GAME_BOARD_WIDTH 6
#define GAME_BOARD_HEIGHT 15
#define GAME_FIRST_VISIBLE_ROW 2
#define GAME_FALLING_GEMS_COUNT 3

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
    GAME_GAME_OVER
} GAME_State;

void GAME_init();

void GAME_update();

extern uint8_t GAME_board[GAME_BOARD_WIDTH][GAME_BOARD_HEIGHT];
extern uint8_t GAME_matches[GAME_BOARD_WIDTH][GAME_BOARD_HEIGHT];

extern GAME_State GAME_state;

extern uint8_t GAME_currentGems[GAME_FALLING_GEMS_COUNT];
extern uint8_t GAME_nextGems[GAME_FALLING_GEMS_COUNT];

extern uint8_t GAME_level;

extern uint8_t GAME_gemsFallTimer;
extern uint8_t GAME_gemsFallTimerHalf;

extern uint8_t GAME_gemX;
extern uint8_t GAME_gemY;  // Y coordinate (board row) of bottom gem.

#endif
