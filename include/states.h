#pragma once

//Which button was pressed?
enum ButtonEvent {
    BTN_RED,
    BTN_YELLOW,
    BTN_BLUE,
    BTN_GREEN,
    BTN_WHITE,
    BTN_NONE // Of course this is the case that none was pressed
};

enum GameState{
    SIMONSAYS,
    STROOP,
    REFLEX,
    LEADERBOARD,
    MENU,
    DIFFICULTY
};

enum Difficulty {
    EASY,
    MEDIUM,
    HARD
};

extern GameState currentGameState;
extern GameState chosenMenuOption;
