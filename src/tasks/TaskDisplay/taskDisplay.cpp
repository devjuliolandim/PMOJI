#include <Arduino.h>
#include <SPI.h>
#include "taskDisplay.h"
#include "displayPrivate.h"
#include "config.h"
#include "states.h"
#include "globals.h"

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// Força o reset das variáveis estáticas internas dos arquivos de gameplay
void resetGameplayHistories() {
    score = 0; // Garante o reset síncrono do score global
    
    // Forçamos uma renderização inicial passando um estado de reset
    // alterando temporariamente o score para um valor impossível antes do início
    int backupScore = score;
    score = -99; 
    renderSimonSaysGameplay();
    renderReflexGameplay();
    score = backupScore;
}

void taskDisplay(void *params) {
    SPI.begin(TFT_SCLK, TFT_MISO, TFT_MOSI, TFT_CS);
    
    tft.init(240, 320); 
    tft.setRotation(3); 
    tft.invertDisplay(true); 
    tft.fillScreen(CLR_WHITE);

    #ifdef TFT_BACKLIGHT
    pinMode(TFT_BACKLIGHT, OUTPUT);
    digitalWrite(TFT_BACKLIGHT, HIGH);
    #endif

    GameState lastGameState = MENU;
    GameState lastMenuOption = MENU;
    Difficulty lastScreenDifficulty = EASY;
    Difficulty lastDifficultyState = EASY; 
    uint32_t lastBatteryTick = 0;

    drawMenuStatic();
    updateMenuSelection(chosenMenuOption, lastMenuOption);
    lastMenuOption = chosenMenuOption;

    while (true) {
        if (currentGameState != lastGameState) {
            vTaskDelay(pdMS_TO_TICKS(50)); 
            tft.fillScreen(CLR_WHITE);

            // Se entramos em qualquer jogo, limpamos o rastro visual da partida anterior
            if (currentGameState == SIMONSAYS || currentGameState == REFLEX || currentGameState == STROOP) {
                resetGameplayHistories();
            }

            switch (currentGameState) {
                case MENU:
                    drawMenuStatic();
                    updateMenuSelection(chosenMenuOption, chosenMenuOption);
                    lastMenuOption = chosenMenuOption;
                    break;
                case LEADERBOARD:
                    lastScreenDifficulty = screenDifficulty;
                    renderLeaderboard(screenDifficulty);
                    break;
                case DIFFICULTY:
                    lastDifficultyState = currentDifficulty;
                    renderDifficultyMenu(currentDifficulty);
                    break;
                default:
                    break;
            }
            
            lastGameState = currentGameState;
            taskYIELD();
        }
        else {
            switch (currentGameState) {
                case MENU:
                    if (chosenMenuOption != lastMenuOption) {
                        updateMenuSelection(chosenMenuOption, lastMenuOption);
                        lastMenuOption = chosenMenuOption;
                    }
                    break;

                case LEADERBOARD:
                    if (screenDifficulty != lastScreenDifficulty) {
                        renderLeaderboard(screenDifficulty);
                        lastScreenDifficulty = screenDifficulty;
                    }
                    break;

                case DIFFICULTY:
                    if (currentDifficulty != lastDifficultyState) {
                        updateDifficultySelection(currentDifficulty, lastDifficultyState);
                        lastDifficultyState = currentDifficulty;
                    }
                    break;

                case SIMONSAYS:
                    renderSimonSaysGameplay();
                    break;

                case STROOP:
                    renderStroopGameplay();
                    break;

                case REFLEX:
                    renderReflexGameplay();
                    break;

                default:
                    break;
            }
        }

        if (millis() - lastBatteryTick >= 5000) {
            updateBatteryIndicator();
            lastBatteryTick = millis();
        }

        vTaskDelay(pdMS_TO_TICKS(33)); 
    }
}