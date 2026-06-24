#include <Arduino.h>
#include "displayPrivate.h"
#include "globals.h"

static void drawGameHeaderUniversal() {
    tft.setTextSize(2); 
    const char* title = (chosenMenuOption == STROOP) ? "STROOP GAME" : 
                        (chosenMenuOption == REFLEX) ? "REFLEX MODE" : "SIMON SAYS";
    
    uint16_t headerColor = CLR_BLUE; 
    if (currentDifficulty == MEDIUM) headerColor = CLR_YELLOW;
    if (currentDifficulty == HARD)   headerColor = CLR_RED;

    int16_t x1, y1;
    uint16_t w, h;
    tft.getTextBounds(title, 0, 0, &x1, &y1, &w, &h);
    int currentX = (tft.width() - w) / 2;

    for (size_t i = 0; i < strlen(title); i++) {
        char letter[2] = {title[i], '\0'};
        drawTextWithOutline(letter, currentX, 16, headerColor);
        currentX += 13; 
    }
    tft.drawFastHLine(0, 46, tft.width(), CLR_GREY);
}

void renderSimonSaysGameplay() {
    static int lastSimonScore = -1;

    // Intercepta o comando de reset da taskDisplay
    if (score == -99) {
        lastSimonScore = -1;
        return;
    }

    if (score != lastSimonScore) {
        tft.fillRect(0, 48, tft.width(), tft.height() - 48, CLR_WHITE);
        drawGameHeaderUniversal();

        uint16_t dynamicColors[] = {CLR_RED, CLR_YELLOW, CLR_BLUE, CLR_GREEN};
        uint16_t randomColor = dynamicColors[abs(score) % 4]; 

        char scoreStr[8];
        sprintf(scoreStr, "%02d", score);

        tft.setTextSize(6); 
        int16_t x1, y1;
        uint16_t w, h;
        tft.getTextBounds(scoreStr, 0, 0, &x1, &y1, &w, &h);
        
        drawTextWithOutline(scoreStr, (tft.width() - w) / 2, 110, randomColor);
        
        lastSimonScore = score;
    }
    updateBatteryIndicator();
}