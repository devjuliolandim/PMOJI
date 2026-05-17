#pragma once

//Which button was pressed?
enum ButtonEvent {
    BTN_RED,
    BTN_BLUE,
    BTN_YELLOW,
    BTN_GREEN,
    BTN_WHITE,
    BTN_NONE // Of course this is the case that none was pressed
};

enum GameState{
    MENU,
    SIMONSAYS,
    STROOP,
    REFLEX,
    LEADERBOARD
};