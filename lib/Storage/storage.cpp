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

int getReflexHighScore(){
    Preferences preferences;
    
    preferences.begin("scores", false);
    
    int score = preferences.getInt("reflex",0);

    preferences.end();

    return score;
}

void saveReflexHighScore(int score){
    Preferences preferences;

    preferences.begin("scores", false);

    preferences.putInt("reflex", score);

    preferences.end();
}