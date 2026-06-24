#include <Arduino.h>
#include "displayPrivate.h"
#include "globals.h" 

// Definição da struct movida para o topo: Visível apenas neste módulo
struct DiffButtonConfig {
    uint16_t color;
    const char* label;
    uint16_t textCol;
};

const int BTN_W = 240;
const int BTN_H = 32;
const int BTN_X = (320 - BTN_W) / 2; 
const int Y_START = 60;
const int Y_SPACING = 42;

const int DIFF_BTN_W = 240;
const int DIFF_BTN_H = 36;
const int DIFF_BTN_X = (320 - DIFF_BTN_W) / 2;
const int DIFF_Y_START = 65;
const int DIFF_Y_SPACING = 48;

static const DiffButtonConfig diffConfigs[] = {
    { CLR_RED,    "DIFICIL (O_O)", CLR_WHITE }, 
    { CLR_YELLOW, "MEDIO  (._.)", CLR_BLACK }, 
    { CLR_BLUE,   "FACIL  (^_-)", CLR_WHITE }  
};

void drawMenuStatic() {
    tft.setTextSize(3);
    const char* title = "P-MOJI";
    uint16_t titleColors[] = {CLR_RED, CLR_YELLOW, CLR_BLUE, CLR_GREEN, CLR_RED, CLR_BLUE};
    
    int16_t x1, y1;
    uint16_t w, h;
    tft.getTextBounds(title, 0, 0, &x1, &y1, &w, &h);
    int startX = (tft.width() - w) / 2;
    int currentX = startX;

    for (int i = 0; i < 6; i++) {
        char letter[2] = {title[i], '\0'};
        drawTextWithOutline(letter, currentX, 12, titleColors[i]);
        currentX += 18; 
    }

    tft.drawFastHLine(0, 46, tft.width(), CLR_GREY);

    struct ButtonConfig {
        GameState opt;
        uint16_t color;
        const char* label;
        uint16_t textCol;
    };

    ButtonConfig configs[] = {
        {SIMONSAYS,   CLR_RED,    "Simon Says", CLR_WHITE},
        {STROOP,     CLR_YELLOW, "Stroop Game", CLR_BLACK},
        {REFLEX,     CLR_BLUE,   "Reflex Mode", CLR_WHITE},
        {LEADERBOARD, CLR_GREEN,  "Leaderboard", CLR_BLACK}
    };

    tft.setTextSize(2);
    for (int i = 0; i < 4; i++) {
        int y = Y_START + (i * Y_SPACING);
        tft.fillRect(BTN_X, y, BTN_W, BTN_H, configs[i].color);
        tft.getTextBounds(configs[i].label, 0, 0, &x1, &y1, &w, &h);
        tft.setCursor(BTN_X + (BTN_W - w) / 2, y + (BTN_H - h) / 2);
        tft.setTextColor(configs[i].textCol);
        tft.print(configs[i].label);
    }
    updateBatteryIndicator();
}

void updateMenuSelection(GameState currentOpt, GameState lastOpt) {
    int lastIdx = (lastOpt == STROOP) ? 1 : (lastOpt == REFLEX) ? 2 : (lastOpt == LEADERBOARD) ? 3 : 0;
    int lastY = Y_START + (lastIdx * Y_SPACING);

    tft.drawRect(BTN_X - 3, lastY - 3, BTN_W + 6, BTN_H + 6, CLR_WHITE);
    tft.drawRect(BTN_X - 2, lastY - 2, BTN_W + 4, BTN_H + 4, CLR_WHITE);
    tft.drawRect(BTN_X - 1, lastY - 1, BTN_W + 2, BTN_H + 2, CLR_WHITE);

    int currentIdx = (currentOpt == STROOP) ? 1 : (currentOpt == REFLEX) ? 2 : (currentOpt == LEADERBOARD) ? 3 : 0;
    int newY = Y_START + (currentIdx * Y_SPACING);

    tft.drawRect(BTN_X - 1, newY - 1, BTN_W + 2, BTN_H + 2, CLR_BLACK);
    tft.drawRect(BTN_X - 2, newY - 2, BTN_W + 4, BTN_H + 4, CLR_BLACK);
    tft.drawRect(BTN_X - 3, newY - 3, BTN_W + 6, BTN_H + 6, CLR_BLACK);
}

void renderDifficultyMenu(Difficulty currentDiff) {
    tft.setTextSize(2);
    const char* title = "DIFICULDADE";
    uint16_t colors[] = {CLR_RED, CLR_YELLOW, CLR_BLUE, CLR_GREEN, CLR_RED, CLR_YELLOW, CLR_BLUE, CLR_GREEN, CLR_RED, CLR_YELLOW, CLR_BLUE};
    
    int16_t x1, y1;
    uint16_t w, h;
    tft.getTextBounds(title, 0, 0, &x1, &y1, &w, &h);
    int currentX = (tft.width() - w) / 2;

    for (int i = 0; i < 11; i++) {
        char letter[2] = {title[i], '\0'};
        drawTextWithOutline(letter, currentX, 16, colors[i]);
        currentX += 13; 
    }
    tft.drawFastHLine(0, 46, tft.width(), CLR_GREY);

    for (int i = 0; i < 3; i++) {
        int y = DIFF_Y_START + (i * DIFF_Y_SPACING);
        tft.fillRect(DIFF_BTN_X, y, DIFF_BTN_W, DIFF_BTN_H, diffConfigs[i].color);
        tft.getTextBounds(diffConfigs[i].label, 0, 0, &x1, &y1, &w, &h);
        tft.setCursor(DIFF_BTN_X + (DIFF_BTN_W - w) / 2, y + (DIFF_BTN_H - h) / 2);
        tft.setTextColor(diffConfigs[i].textCol);
        tft.print(diffConfigs[i].label);
    }

    updateDifficultySelection(currentDiff, currentDiff);
    updateBatteryIndicator();
}

void updateDifficultySelection(Difficulty currentDiff, Difficulty lastDiff) {
    int lastVisualIdx = 2 - (int)lastDiff;
    int lastY = DIFF_Y_START + (lastVisualIdx * DIFF_Y_SPACING);
    
    tft.drawRect(DIFF_BTN_X - 3, lastY - 3, DIFF_BTN_W + 6, DIFF_BTN_H + 6, CLR_WHITE);
    tft.drawRect(DIFF_BTN_X - 2, lastY - 2, DIFF_BTN_W + 4, DIFF_BTN_H + 4, CLR_WHITE);
    tft.drawRect(DIFF_BTN_X - 1, lastY - 1, DIFF_BTN_W + 2, DIFF_BTN_H + 2, CLR_WHITE);

    int currentVisualIdx = 2 - (int)currentDiff;
    int currentY = DIFF_Y_START + (currentVisualIdx * DIFF_Y_SPACING);
    
    tft.drawRect(DIFF_BTN_X - 1, currentY - 1, DIFF_BTN_W + 2, DIFF_BTN_H + 2, CLR_BLACK);
    tft.drawRect(DIFF_BTN_X - 2, currentY - 2, DIFF_BTN_W + 4, DIFF_BTN_H + 4, CLR_BLACK);
    tft.drawRect(DIFF_BTN_X - 3, currentY - 3, DIFF_BTN_W + 6, DIFF_BTN_H + 6, CLR_BLACK);
}