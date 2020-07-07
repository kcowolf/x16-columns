#include <cx16.h>
#include <stdint.h>
#include <stdlib.h>
#include "GAME.h"
#include "GFX.h"
#include "GFX_util.h"
#include "INPT.h"

#define BUTTON_LOCK_FRAMES 7
#define GEMS_FOR_MATCH 3
#define GEMS_INITIAL_COLUMN 3
#define GEMS_INITIAL_ROW 2

#define MAGIC_GEM_INDEX 7

#define LEVEL_FALL_TIMERS_COUNT 1

#define MAGIC_GEM_INITIAL_COUNT 128

#define FLASH_EASY_GEM_MAX_INDEX 3
#define FLASH_NORMAL_GEM_MAX_INDEX 4
#define FLASH_HARD_GEM_MAX_INDEX 5
#define FLASH_MIN_HEIGHT 2
#define FLASH_MAX_HEIGHT 9
#define ORIGINAL_GEM_MAX_INDEX 5

uint8_t GAME_board[GAME_BOARD_WIDTH][GAME_BOARD_HEIGHT];
uint8_t GAME_matches[GAME_BOARD_WIDTH][GAME_BOARD_HEIGHT];

GAME_Mode GAME_mode;
GAME_State GAME_state;
uint8_t GAME_currentGems[GAME_FALLING_GEMS_COUNT];
uint8_t GAME_nextGems[GAME_FALLING_GEMS_COUNT];

uint8_t GAME_level;

uint8_t GAME_gemsFallTimer;
uint8_t GAME_gemsFallTimerHalf;

uint8_t GAME_gemX;
uint8_t GAME_gemY;

uint8_t magicGemCountdown;

uint8_t gemMaxIndex;

static uint8_t checkForMatches();
static void checkInput();
static void clearMatches();
static void gravity();
static void initCommon();
static void setNextGems();
static void setNextLevel();

// TODO -- Set these
const uint8_t levelFallTimers[LEVEL_FALL_TIMERS_COUNT] = { 40 };

void GAME_init_original()
{
    GAME_mode = GAME_ORIGINAL;
    gemMaxIndex = ORIGINAL_GEM_MAX_INDEX;

    initCommon();
}

void GAME_init_flash(GAME_FlashDifficulty difficulty, uint8_t height)
{
    uint8_t x;
    uint8_t y;
    uint8_t val;
    uint8_t matchCount;

    GAME_mode = GAME_FLASH;

    switch (difficulty)
    {
        case GAME_FLASH_EASY:
            gemMaxIndex = FLASH_EASY_GEM_MAX_INDEX;
            break;
        case GAME_FLASH_NORMAL:
            gemMaxIndex = FLASH_NORMAL_GEM_MAX_INDEX;
            break;
        case GAME_FLASH_HARD:
        // fallthrough is intentional
        default:
            gemMaxIndex = FLASH_HARD_GEM_MAX_INDEX;
            break;
    };

    if (height < FLASH_MIN_HEIGHT)
    {
        height = FLASH_MIN_HEIGHT;
    }

    if (height > FLASH_MAX_HEIGHT)
    {
        height = FLASH_MAX_HEIGHT;
    }

    // Call initCommon() here to clear out the board and match arrays.
    // Must be done before adding height lines.
    initCommon();

    // Add height lines
    do
    {
        for (x = 0; x < GAME_BOARD_WIDTH; x++)
        {
            for (y = GAME_BOARD_HEIGHT - 1; y > GAME_BOARD_HEIGHT - height - 1; y--)
            {
                if (GAME_board[x][y] == 0)
                {
                    do
                    {
                        val = rand() & 0x7;
                    } while (val > gemMaxIndex);

                    GAME_board[x][y] = val + 1;
                }
            }
        }

        matchCount = checkForMatches();
        clearMatches();
    } while(matchCount != 0);
}

void GAME_update()
{
    uint8_t x;
    uint8_t y;

    switch (GAME_state)
    {
        case GAME_INIT:
            break;
        case GAME_WAIT_FOR_START:
            // TODO -- Count down for 5 seconds.
            GAME_state = GAME_INIT_GEMS;
            break;
        case GAME_INIT_GEMS:
            for (x = 0; x < GAME_BOARD_WIDTH; x++)
            {
                for (y = 0; y < GAME_FIRST_VISIBLE_ROW; y++)
                {
                    if (GAME_board[x][y] != 0)
                    {
                        GAME_state = GAME_GAME_OVER;
                        return;
                    }
                }
            }

            if (GAME_board[GEMS_INITIAL_COLUMN][GAME_FIRST_VISIBLE_ROW] != 0)
            {
                GAME_state = GAME_GAME_OVER;
                return;
            }

            GAME_gemX = GEMS_INITIAL_COLUMN;
            GAME_gemY = GAME_FIRST_VISIBLE_ROW;
            setNextGems();

            GAME_gemsFallTimer = levelFallTimers[GAME_level];
            GAME_gemsFallTimerHalf = GAME_gemsFallTimer >> 1;
            GAME_state = GAME_GEMS_INITIALIZED;
            break;
        case GAME_GEMS_INITIALIZED:
            GAME_state = GAME_GEMS_FALLING;
            // Fallthrough is intentional.
        case GAME_GEMS_FALLING:
            GAME_gemsFallTimer--;

            if (GAME_gemsFallTimer == 0)
            {
                if (GAME_gemY + 1 >= GAME_BOARD_HEIGHT || GAME_board[GAME_gemX][GAME_gemY + 1] != 0)
                {
                    // Gems are locked
                    GAME_board[GAME_gemX][GAME_gemY - 2] = GAME_currentGems[0];
                    GAME_board[GAME_gemX][GAME_gemY - 1] = GAME_currentGems[1];
                    GAME_board[GAME_gemX][GAME_gemY] = GAME_currentGems[2];
                    GAME_state = GAME_CHECK_FOR_MATCHES;
                    return;
                }
                else
                {
                    GAME_gemY++;
                    GAME_gemsFallTimer = levelFallTimers[GAME_level];
                }
            }

            checkInput();
            break;
        case GAME_CHECK_FOR_MATCHES:
        {
            uint8_t matchCount = checkForMatches();
            if (matchCount > 0)
            {
                GAME_state = GAME_WAIT_FOR_MATCHES;
            }
            else
            {
                GAME_state = GAME_INIT_GEMS;
            }
            break;
        }
        case GAME_WAIT_FOR_MATCHES:
            break;
        case GAME_CLEAR_MATCHES:
            clearMatches();
            GAME_state = GAME_MATCHES_CLEARED;
            break;
        case GAME_MATCHES_CLEARED:
            GAME_state = GAME_CHECK_FOR_MATCHES;
            break;
        case GAME_GAME_OVER:
            GAME_state = GAME_WAIT_FOR_GAME_OVER;
            break;
        case GAME_WAIT_FOR_GAME_OVER:
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

    if (GAME_currentGems[0] == MAGIC_GEM_INDEX)
    {
        GAME_matches[GAME_gemX][GAME_gemY - 2] = 1;
        GAME_matches[GAME_gemX][GAME_gemY - 1] = 1;
        GAME_matches[GAME_gemX][GAME_gemY] = 1;
        matchesTotal = 3;
        GAME_currentGems[0] = 0; // So we stop processing magic gems.

        if (GAME_gemY + 1 != GAME_BOARD_HEIGHT)
        {
            uint8_t matchColor = GAME_board[GAME_gemX][GAME_gemY + 1];
            for (x = 0; x < GAME_BOARD_WIDTH; x++)
            {
                for (y = 0; y < GAME_BOARD_HEIGHT; y++)
                {
                    if (GAME_board[x][y] == matchColor)
                    {
                        GAME_matches[x][y] = 1;
                        matchesTotal++;
                    }
                }
            }
        }
    }
    else
    {
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
    }

    return matchesTotal;
}

static void checkInput()
{
    if (INPT_state & INPT_DOWN_MASK)
    {
        if (GAME_gemY + 1 < GAME_BOARD_HEIGHT && GAME_board[GAME_gemX][GAME_gemY + 1] == 0)
        {
            GAME_gemY++;
            GAME_gemsFallTimer = GAME_gemsFallTimerHalf;
        }
        else if (GAME_gemsFallTimer > GAME_gemsFallTimerHalf)
        {
            GAME_gemsFallTimer = GAME_gemsFallTimerHalf;
        }

        INPT_lockUpDown(BUTTON_LOCK_FRAMES);
    }

    if (INPT_state & INPT_RIGHT_MASK)
    {
        if (GAME_gemX + 1 < GAME_BOARD_WIDTH && GAME_board[GAME_gemX + 1][GAME_gemY] == 0)
        {
            GAME_gemX++;
        }

        INPT_lockLeftRight(BUTTON_LOCK_FRAMES);
    }
    else if (INPT_state & INPT_LEFT_MASK) // Right takes priority if both are pressed.
    {
        if (GAME_gemX != 0 && GAME_board[GAME_gemX - 1][GAME_gemY] == 0)
        {
            GAME_gemX--;
        }

        INPT_lockLeftRight(BUTTON_LOCK_FRAMES);
    }

    if (INPT_state & INPT_ACTION_MASK)
    {
        uint8_t gem = GAME_currentGems[2];
        GAME_currentGems[2] = GAME_currentGems[1];
        GAME_currentGems[1] = GAME_currentGems[0];
        GAME_currentGems[0] = gem;

        INPT_lockAction();
    }
}

static void clearMatches()
{
    int8_t x;
    int8_t y;
    int8_t checkY;

    // Clear matches from the board
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

    // Gravity
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

static void initCommon()
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

    setNextGems();  // Initialize next.

    GAME_level = 0;
    magicGemCountdown = MAGIC_GEM_INITIAL_COUNT;

    GAME_state = GAME_INIT;
}

static void setNextGems()
{
    uint8_t i;
    uint8_t val;

    if (GAME_mode == GAME_ORIGINAL)
    {
        magicGemCountdown--;
        if (magicGemCountdown == 0)
        {
            for (i = 0; i < GAME_FALLING_GEMS_COUNT; i++)
            {
                GAME_currentGems[i] = GAME_nextGems[i];
                GAME_nextGems[i] = MAGIC_GEM_INDEX;
            }

            magicGemCountdown = MAGIC_GEM_INITIAL_COUNT;
            return;
        }
    }

    for (i = 0; i < GAME_FALLING_GEMS_COUNT; i++)
    {
        GAME_currentGems[i] = GAME_nextGems[i];
        {
            do
            {
                val = rand() & 0x7;
            } while (val > gemMaxIndex);

            // val will be between 0 and gemMaxIndex.
            GAME_nextGems[i] = val + 1;
        }
    }
}

static void setNextLevel()
{
    if (GAME_level != 0xFF)
    {
        GAME_level++;
    }
}
