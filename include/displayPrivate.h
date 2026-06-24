#ifndef DISPLAY_PRIVATE_H
#define DISPLAY_PRIVATE_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include "states.h"
#include "globals.h"
#include "taskDifficulty.h"

// Cores lógicas globais do display
#ifndef CLR_BLACK
#define CLR_BLACK   0xFFFF
#endif
#ifndef CLR_WHITE
#define CLR_WHITE   0x0000
#endif
#ifndef CLR_RED
#define CLR_RED     0x07FF
#endif
#ifndef CLR_GREEN
#define CLR_GREEN   0xF81F
#endif
#ifndef CLR_BLUE
#define CLR_BLUE    0xFFE0
#endif
#ifndef CLR_YELLOW
#define CLR_YELLOW  0x001F
#endif
#ifndef CLR_GREY
#define CLR_GREY    0x8410
#endif

extern Adafruit_ST7789 tft;

// Funções compartilhadas com o orquestrador
void drawMenuStatic();
void updateMenuSelection(GameState current, GameState last);
void renderLeaderboard(Difficulty diff);
void renderDifficultyMenu(Difficulty current);
void updateDifficultySelection(Difficulty current, Difficulty last);

void renderSimonSaysGameplay();
void renderStroopGameplay();
void renderReflexGameplay();

void updateBatteryIndicator();
void drawTextWithOutline(const char* text, int16_t x, int16_t y, uint16_t color);

#endif