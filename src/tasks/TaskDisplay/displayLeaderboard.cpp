#include <Arduino.h>
#include "displayPrivate.h"
#include "storage.h"
#include "globals.h"

static void drawLeaderboardHeader() {
    tft.setTextSize(3);
    const char* title = "RECORDES";
    uint16_t colors[] = {CLR_RED, CLR_YELLOW, CLR_BLUE, CLR_GREEN, CLR_RED, CLR_YELLOW, CLR_BLUE, CLR_GREEN};
    
    int16_t x1, y1;
    uint16_t w, h;
    tft.getTextBounds(title, 0, 0, &x1, &y1, &w, &h);
    int startX = (tft.width() - w) / 2;
    int currentX = startX;

    for (int i = 0; i < 8; i++) {
        char letter[2] = {title[i], '\0'};
        drawTextWithOutline(letter, currentX, 12, colors[i]);
        currentX += 18; 
    }
    tft.drawFastHLine(0, 46, tft.width(), CLR_GREY);
}

static void drawScoreRow(const char* gameName, const char* gameId, Difficulty diff, int yPos) {
    int16_t x1, y1;
    uint16_t w, h;
    char scoreStr[16];

    Difficulty backupDiff = currentDifficulty;
    currentDifficulty = diff; 
    int highVal = getScore(gameId);
    currentDifficulty = backupDiff; 

    sprintf(scoreStr, "%03d PTS", highVal);

    tft.drawRect(15, yPos - 6, tft.width() - 30, 36, CLR_GREY);
    tft.setTextSize(2);
    tft.setTextColor(CLR_BLACK);
    tft.setCursor(25, yPos + 4);
    tft.print(gameName);

    tft.getTextBounds(scoreStr, 0, 0, &x1, &y1, &w, &h);
    int scoreX = tft.width() - w - 25;
    
    tft.setTextColor(CLR_RED); 
    tft.setCursor(scoreX, yPos + 4);
    tft.print(scoreStr);
}

void renderLeaderboard(Difficulty diff) {
    tft.fillRect(0, 48, tft.width(), tft.height() - 48, CLR_WHITE);
    drawLeaderboardHeader();

    tft.setTextSize(2);
    tft.setTextColor(CLR_BLACK);
    const char* diffStr = (diff == EASY) ? "EASY" : (diff == MEDIUM) ? "MEDIUM" : "HARD";
    char subStr[32];
    sprintf(subStr, "DIFICULDADE: %s", diffStr);
    
    int16_t x1, y1;
    uint16_t w, h;
    tft.getTextBounds(subStr, 0, 0, &x1, &y1, &w, &h);
    tft.setCursor((tft.width() - w) / 2, 58);
    tft.print(subStr);

    int startY = 95;
    int rowSpacing = 46;

    drawScoreRow("Simon Says", "simon", diff, startY);                    
    drawScoreRow("Reflex Mode", "reflex", diff, startY + rowSpacing);      
    drawScoreRow("Stroop Game", "stroop", diff, startY + (rowSpacing * 2));
    updateBatteryIndicator();
}