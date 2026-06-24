#include <Arduino.h>
#include "displayPrivate.h"
#include "globals.h"

extern int randomLed;        
extern const char* stroopWord; 

static inline uint16_t clarearCorRGB565(uint16_t cor) {
    uint16_t c = ~cor; 
    uint16_t r = (c >> 11) & 0x1F;
    uint16_t g = (c >> 5) & 0x3F;
    uint16_t b = c & 0x1F;

    r = (r + 16 > 31) ? 31 : r + 16;
    g = (g + 32 > 63) ? 63 : g + 32;
    b = (b + 16 > 31) ? 31 : b + 16;

    return (uint16_t)~( (r << 11) | (g << 5) | b );
}

static void drawStroopHeader() {
    tft.setTextSize(2);
    const char* title = "EFEITO STROOP";
    
    uint16_t headerColor = CLR_BLUE;
    if (currentDifficulty == MEDIUM) headerColor = CLR_YELLOW;
    if (currentDifficulty == HARD)   headerColor = CLR_RED;

    int16_t x1, y1;
    uint16_t w, h;
    tft.getTextBounds(title, 0, 0, &x1, &y1, &w, &h);
    int centerX = (tft.width() - w) / 2;

    drawTextWithOutline(title, centerX, 16, headerColor);
    tft.drawFastHLine(0, 46, tft.width(), CLR_GREY);
}

void renderStroopGameplay() {
    static int lastScore = -1;
    static int lastColor = -1;
    static char lastWordBuffer[32] = ""; 

    const char* currentWord = (stroopWord != nullptr) ? stroopWord : "";

    if (score != lastScore || randomLed != lastColor || strcmp(currentWord, lastWordBuffer) != 0) {
        uint16_t bgClaro = clarearCorRGB565(randomLed);
        
        // Sempre limpa o bloco de conteúdo inteiro com a nova cor para evitar o artefato de quadrado fantasma
        tft.fillRect(0, 48, tft.width(), tft.height() - 48, bgClaro);
        drawStroopHeader();

        if (strlen(currentWord) > 0) {
            tft.setTextSize(4); 
            int16_t x1, y1;
            uint16_t w, h;
            tft.getTextBounds(currentWord, 0, 0, &x1, &y1, &w, &h);
            drawTextWithOutline(currentWord, (tft.width() - w) / 2, 110, randomLed);
        }

        tft.setTextSize(2);
        char scoreStr[16];
        sprintf(scoreStr, "SCORE: %02d", score);
        drawTextWithOutline(scoreStr, 20, 200, CLR_WHITE);

        lastScore = score;
        lastColor = randomLed;
        strncpy(lastWordBuffer, currentWord, sizeof(lastWordBuffer) - 1);
    }
    updateBatteryIndicator();
}