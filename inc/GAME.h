#ifndef GAME_H
#define GAME_H

#define GAME_BOARD_WIDTH 6
#define GAME_BOARD_HEIGHT 16

typedef enum
{
    GAME_INIT,            // SCRN will transition after drawing the board.
    GAME_WAIT_FOR_START
} GAME_State;

void GAME_init();

void GAME_update();

extern uint8_t GAME_board[GAME_BOARD_WIDTH][GAME_BOARD_HEIGHT];
extern uint8_t GAME_matches[GAME_BOARD_WIDTH][GAME_BOARD_HEIGHT];

extern GAME_State GAME_state;

#endif
