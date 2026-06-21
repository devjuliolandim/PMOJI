#include <Arduino.h>
#include "storage.h"
#include "states.h"
#include "queues.h"
#include "taskHandles.h"
#include "taskLeaderBoard.h"
#include "taskDifficulty.h"

const char* difficulties[3] = {"EASY","MEDIUM","HARD"};
const char* gameNames[3] = {"Simon Says", "Reflexo", "Stroop"};
const char* gameIds[3] = {"simon","reflex","stroop"};

void taskLeaderBoard(void * params){
    
    ButtonEvent receivedButton;
    bool shouldPrint = true;
    
    while(true){

        if(shouldPrint){
            Serial.println("RECORDES: ");  

            for(int i = 0; i < 3;i++){
                Serial.println(difficulties[i]);
                currentDifficulty = (Difficulty) i;
                for(int j = 0; j < 3; j++){
                    Serial.print(gameNames[j]);
                    Serial.print(": ");
                    Serial.println(getScore(gameIds[j]));
                }
                Serial.println("\n");
            }
            shouldPrint = false;
        }

        if(xQueueReceive(inputQueue, &receivedButton, portMAX_DELAY)== pdTRUE && receivedButton == BTN_WHITE){
            vTaskResume(menuTaskHandle);
            vTaskSuspend(leaderboardTaskHandle);
            shouldPrint = true;
        }


    }
}