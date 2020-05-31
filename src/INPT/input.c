#include <cx16.h>
#include <joystick.h>
#include <stdbool.h>
#include "INPT.h"

uint8_t INPT_state;
static uint8_t leftRightLock;
static uint8_t upDownLock;
static bool actionLock;

static const unsigned char joyMasks[INPT_BUTTON_COUNT] =
{
    JOY_UP_MASK,
    JOY_DOWN_MASK,
    JOY_LEFT_MASK,
    JOY_RIGHT_MASK,
    JOY_BTN_1_MASK
};

void INPT_clearLocks()
{
    uint8_t i;

    leftRightLock = 0;
    upDownLock = 0;
    actionLock = false;
}

void INPT_init()
{
    INPT_state = 0;

    INPT_clearLocks();
}

void INPT_update()
{
    uint8_t i;
    uint8_t joyStatus = joy_read(0);
    INPT_state = 0;

    if (leftRightLock > 0)
    {
        leftRightLock--;
    }

    if (upDownLock > 0)
    {
        upDownLock--;
    }

    if (joyStatus & JOY_RIGHT_MASK)
    {
        if (leftRightLock == 0)
        {
            INPT_state |= INPT_RIGHT_MASK;
        }
    }
    else if (joyStatus & JOY_LEFT_MASK)
    {
        if (leftRightLock == 0)
        {
            INPT_state |= INPT_LEFT_MASK;
        }
    }
    else
    {
        leftRightLock = 0;
    }

    if (joyStatus & JOY_DOWN_MASK)
    {
        if (upDownLock == 0)
        {
            INPT_state |= INPT_DOWN_MASK;
        }
    }
    else if (joyStatus & JOY_UP_MASK)
    {
        if (upDownLock == 0)
        {
            INPT_state |= INPT_UP_MASK;
        }
    }
    else
    {
        upDownLock = 0;
    }

    if (joyStatus & JOY_BTN_1_MASK)
    {
        if (!actionLock)
        {
            INPT_state |= INPT_ACTION_MASK;
        }
    }
    else
    {
        actionLock = false;
    }
}

void INPT_lockAction()
{
    actionLock = true;
}

void INPT_lockLeftRight
    (
    uint8_t frames
    )
{
    leftRightLock = frames;
}

void INPT_lockUpDown
    (
    uint8_t frames
    )
{
    upDownLock = frames;
}
