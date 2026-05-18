#include <Arduino.h>
#include <Preferences.h>

#include "storage.h"


int getSimonHighScore(){

    Preferences preferences;

    preferences.begin("scores", true);

    int score = preferences.getInt("simon", 0);

    preferences.end();

    return score;
}

void saveSimonHighScore(int score){

    Preferences preferences;

    preferences.begin("scores", false);

    preferences.putInt("simon", score);

    preferences.end();
}