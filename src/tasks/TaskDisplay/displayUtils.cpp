#include <Arduino.h>
#include "displayPrivate.h"
#include "taskBattery.h" // Acesso à variável global batteryLevel

void drawTextWithOutline(const char* text, int16_t x, int16_t y, uint16_t textColor) {
    tft.setTextColor(CLR_BLACK);
    for (int dx = -2; dx <= 2; dx++) {
        for (int dy = -2; dy <= 2; dy++) {
            if (dx != 0 || dy != 0) {
                tft.setCursor(x + dx, y + dy);
                tft.print(text);
            }
        }
    }
    tft.setTextColor(textColor);
    tft.setCursor(x, y);
    tft.print(text);
}

void updateBatteryIndicator() {
    int pct = (int)((batteryLevel - 3.3) * 100.0 / (4.2 - 3.3));
    if (pct > 100) pct = 100;
    if (pct < 0)   pct = 0;

    char batStr[8];
    sprintf(batStr, "%d%%", pct);

    tft.setTextSize(2);
    int16_t x1, y1;
    uint16_t w, h;
    tft.getTextBounds(batStr, 0, 0, &x1, &y1, &w, &h);
    
    int batX = tft.width() - w - 15;
    tft.fillRect(tft.width() - 75, 12, 70, 20, CLR_WHITE);
    tft.setTextColor(CLR_BLACK); 
    tft.setCursor(batX, 15);
    tft.print(batStr);
}