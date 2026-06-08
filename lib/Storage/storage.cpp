#include <Arduino.h>
#include <Preferences.h>

#include "storage.h"

int getHighscore(char* gameName){
    Preferences preferences;
    
    preferences.begin("scores",true);
    
    int score = preferences.getInt(gameName, 0);
    
    preferences.end();
    
    return score;
}

void saveHighscore(char *gameName, int score){
    Preferences preferences;

    preferences.begin("scores", false);

    preferences.putInt(gameName, score);

    preferences.end();
}
