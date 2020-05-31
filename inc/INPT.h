#ifndef INPT_H
#define INPT_H

#include <stdint.h>

typedef enum
{
    INPT_UP,
    INPT_DOWN,
    INPT_LEFT,
    INPT_RIGHT,
    INPT_ACTION,

    INPT_BUTTON_COUNT
} INPT_ButtonType;

typedef enum
{
    INPT_NONE = 0,
    INPT_UP_MASK = 1 << INPT_UP,
    INPT_DOWN_MASK = 1 << INPT_DOWN,
    INPT_LEFT_MASK = 1 << INPT_LEFT,
    INPT_RIGHT_MASK = 1 << INPT_RIGHT,
    INPT_ACTION_MASK = 1 << INPT_ACTION,
} INPT_MaskType;

void INPT_clearLocks();

void INPT_init();

void INPT_update();

void INPT_lockAction();

void INPT_lockLeftRight
    (
    uint8_t frames
    );

void INPT_lockUpDown
    (
    uint8_t frames
    );

extern uint8_t INPT_state;

#endif
