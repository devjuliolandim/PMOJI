#pragma once
#include "states.h"

// LEDs
#define RED_LED        2
#define BLUE_LED       4
#define YELLOW_LED     5
#define GREEN_LED      18
#define WHITE_LED     19

// Buttons
#define RED_BUTTON     13
#define BLUE_BUTTON    14
#define YELLOW_BUTTON  12
#define GREEN_BUTTON   27
#define WHITE_BUTTON  26

// Buzzer
#define BUZZER         25


//Array of Buttons
int arrButton[5][2] = {
    {RED_BUTTON, BTN_RED},
    {BLUE_BUTTON, BTN_BLUE},
    {YELLOW_BUTTON, BTN_YELLOW},
    {GREEN_BUTTON, BTN_GREEN},
    {WHITE_BUTTON, BTN_WHITE}
};