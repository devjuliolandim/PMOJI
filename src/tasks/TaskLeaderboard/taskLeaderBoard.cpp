#include <Arduino.h>
#include "storage.h"
#include "states.h"
#include "queues.h"
#include "taskHandles.h"
#include "taskLeaderBoard.h"

void taskLeaderBoard(void * params){
    
    ButtonEvent receivedButton;
    bool shouldPrint = true;
    
    while(true){

        if(shouldPrint){
            Serial.println("RECORDES: ");   
            Serial.print("Simon Says: ");
            Serial.println(getScore("simon"));
            Serial.print("Reflex: ");
            Serial.println(getScore("reflex"));
            shouldPrint = false;
        }

        if(xQueueReceive(inputQueue, &receivedButton, portMAX_DELAY)== pdTRUE && receivedButton == BTN_WHITE){
            vTaskResume(menuTaskHandle);
            vTaskSuspend(leaderboardTaskHandle);
            shouldPrint = true;
        }


    }
}