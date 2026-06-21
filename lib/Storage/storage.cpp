#include <Arduino.h>
#include <Preferences.h>
#include <string>

#include "storage.h"
#include "../../include/taskDifficulty.h"

const char* handleGameId(const char* gameId) {
    static std::string result;

    switch (currentDifficulty)
    {
    case EASY:
        result = std::string(gameId) + "Easy";
        break;

    case MEDIUM:
        result = std::string(gameId) + "Medium";
        break;

    case HARD:
        result = std::string(gameId) + "Hard";
        break;

    default:
        result = gameId;
        break;
    }

    return result.c_str();
}

int getScore(const char *gameId){
    Preferences preferences;

    preferences.begin("scores", true);

    int score = preferences.getInt(handleGameId(gameId), 0);

    preferences.end();
    
    return score;
}

void saveScore(const char *gameId, int score){
    Preferences preferences;

    preferences.begin("scores", false);

    preferences.putInt(handleGameId(gameId), score);

    preferences.end();
}
