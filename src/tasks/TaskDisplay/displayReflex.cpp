#include <Arduino.h>
#include "displayPrivate.h"
#include "globals.h"

extern int randomLed; 

static void drawReflexHeader() {
    tft.setTextSize(2);
    const char* title = "MODO REFLEXO";
    
    uint16_t headerColor = CLR_BLUE;
    if (currentDifficulty == MEDIUM) headerColor = CLR_YELLOW;
    if (currentDifficulty == HARD)   headerColor = CLR_RED;

    int16_t x1, y1;
    uint16_t w, h;
    tft.getTextBounds(title, 0, 0, &x1, &y1, &w, &h);
    
    drawTextWithOutline(title, (tft.width() - w) / 2, 16, headerColor);
    tft.drawFastHLine(0, 46, tft.width(), CLR_GREY);
}

void renderReflexGameplay() {
    static int lastScore = -1;
    static int lastColor = -1;
    static uint16_t currentColor = CLR_WHITE;

    // Intercepta o comando de reset da taskDisplay
    if (score == -99) {
        lastScore = -1;
        lastColor = -1;
        return;
    }

    if (score != lastScore || randomLed != lastColor) {
        if (score != lastScore) {
            uint16_t dynamicColors[] = {CLR_RED, CLR_YELLOW, CLR_BLUE, CLR_GREEN};
            currentColor = dynamicColors[random(0, 4)];
        }

        tft.fillRect(0, 48, tft.width(), tft.height() - 48, CLR_WHITE);
        drawReflexHeader();

        char scoreStr[8];
        sprintf(scoreStr, "%02d", score);
        tft.setTextSize(6); 
        int16_t x1, y1;
        uint16_t w, h;
        tft.getTextBounds(scoreStr, 0, 0, &x1, &y1, &w, &h);
        
        drawTextWithOutline(scoreStr, (tft.width() - w) / 2, 110, currentColor);

        lastScore = score;
        lastColor = randomLed;
    }
    updateBatteryIndicator();
}