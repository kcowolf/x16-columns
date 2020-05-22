#include <cx16.h>
#include <stdint.h>
#include "GAME.h"
#include "GFX.h"
#include "GFX_util.h"

#define BANKED_RAM_START 0xA000
#define GEMS_FOR_MATCH 3

uint8_t GAME_board[GAME_BOARD_WIDTH][GAME_BOARD_HEIGHT];
uint8_t GAME_matches[GAME_BOARD_WIDTH][GAME_BOARD_HEIGHT];
GAME_State GAME_state;

static uint8_t checkForMatches();
static void clearMatches();
static void gravity();

void GAME_init()
{
    uint8_t x;
    uint8_t y;

    for (x = 0; x < GAME_BOARD_WIDTH; x++)
    {
        for (y = 0; y < GAME_BOARD_HEIGHT; y++)
        {
            GAME_board[x][y] = 0;
            GAME_matches[x][y] = 0;
        }
    }

    GAME_board[0][3] = 1;
    GAME_board[1][3] = 2;
    GAME_board[2][3] = 3;
    GAME_board[3][3] = 4;
    GAME_board[4][3] = 5;
    GAME_board[5][3] = 6;

    GAME_board[0][15] = 1;
    GAME_board[1][15] = 2;
    GAME_board[2][15] = 3;
    GAME_board[3][15] = 4;
    GAME_board[4][15] = 5;
    GAME_board[5][15] = 6;

    GAME_state = GAME_INIT;
}

void GAME_update()
{
    switch (GAME_state)
    {
        case GAME_INIT:
            break;
        case GAME_WAIT_FOR_START:
            break;
    }
}

static uint8_t checkForMatches()
{
    int8_t x;
    int8_t y;
    int8_t checkX;
    int8_t checkY;
    uint8_t currentMatches;

    uint8_t matchesTotal = 0;

    for (x = 0; x < GAME_BOARD_WIDTH; x++)
    {
        for (y = 0; y < GAME_BOARD_HEIGHT; y++)
        {
            if (GAME_board[x][y] != 0)
            {
                // Check down.
                currentMatches = 1;
                checkY = y + 1;
                while (checkY < GAME_BOARD_HEIGHT && GAME_board[x][checkY] == GAME_board[x][y])
                {
                    currentMatches++;
                    checkY++;
                }

                if (currentMatches >= GEMS_FOR_MATCH)
                {
                    matchesTotal += currentMatches;

                    do
                    {
                        checkY--;
                        GAME_matches[x][checkY] = 1;
                    } while (checkY != y);
                }

                // Check right.
                currentMatches = 1;
                checkX = x + 1;
                while (checkX < GAME_BOARD_WIDTH && GAME_board[checkX][y] == GAME_board[x][y])
                {
                    currentMatches++;
                    checkX++;
                }

                if (currentMatches >= GEMS_FOR_MATCH)
                {
                    matchesTotal += currentMatches;

                    do
                    {
                        checkX--;
                        GAME_matches[checkX][y] = 1;
                    } while (checkX != x);
                }

                // Check down-right.
                currentMatches = 1;
                checkX = x + 1;
                checkY = y + 1;
                while (checkX < GAME_BOARD_WIDTH && checkY < GAME_BOARD_HEIGHT && GAME_board[checkX][checkY] == GAME_board[x][y])
                {
                    currentMatches++;
                    checkX++;
                    checkY++;
                }

                if (currentMatches >= GEMS_FOR_MATCH)
                {
                    matchesTotal += currentMatches;

                    do
                    {
                        checkX--;
                        checkY--;
                        GAME_matches[checkX][checkY] = 1;
                    } while (checkX != x);
                }

                // Check up-right.
                currentMatches = 1;
                checkX = x + 1;
                checkY = y - 1;
                while (checkX < GAME_BOARD_WIDTH && checkY >= 0 && GAME_board[checkX][checkY] == GAME_board[x][y])
                {
                    currentMatches++;
                    checkX++;
                    checkY--;
                }

                if (currentMatches >= GEMS_FOR_MATCH)
                {
                    matchesTotal += currentMatches;

                    do
                    {
                        checkX--;
                        checkY++;
                        GAME_matches[checkX][checkY] = 1;
                    } while (checkX != x);
                }
            }
        }
    }

    return matchesTotal;
}

static void clearMatches()
{
    int8_t x;
    int8_t y;

    for (x = 0; x < GAME_BOARD_WIDTH; x++)
    {
        for (y = 0; y < GAME_BOARD_HEIGHT; y++)
        {
            if (GAME_matches[x][y] != 0)
            {
                GAME_board[x][y] = 0;
                GAME_matches[x][y] = 0;
            }
        }
    }
}

void gravity()
{
    int8_t x;
    int8_t y;
    int8_t checkY;

    for (x = 0; x < GAME_BOARD_WIDTH; x++)
    {
        for (y = GAME_BOARD_HEIGHT - 2; y >= 0; y--)
        {
            if (GAME_board[x][y] != 0)
            {
                checkY = y + 1;
                while (checkY < GAME_BOARD_HEIGHT && GAME_board[x][checkY] == 0)
                {
                    checkY++;
                }

                if (checkY - 1 != y)
                {
                    GAME_board[x][checkY - 1] = GAME_board[x][y];
                    GAME_board[x][y] = 0;
                }
            }
        }
    }
}
