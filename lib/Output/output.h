#pragma once

#include <Arduino.h>

#define GAME_BEGIN_DELAY 200

void blinkAndBuzzer(int index, int delay);
void gameBeginRoutine();
void gameOverRoutine();
void handleCorrectInputSound(bool isCorrect, int led);
void pmojiOpening();