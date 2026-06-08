#include <Arduino.h>
#include <Preferences.h>

#include "storage.h"

int getScore(const char *gameId){
    Preferences preferences;

    preferences.begin("scores", true);

    int score = preferences.getInt(gameId, 0);

    preferences.end();

    return score;
}

void saveScore(const char *gameId, int score){
    Preferences preferences;

    preferences.begin("scores", false);

    preferences.putInt(gameId, score);

    preferences.end();
}