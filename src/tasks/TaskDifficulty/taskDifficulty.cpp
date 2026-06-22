#include <Arduino.h>

#include "queues.h"
#include "states.h"
#include "taskDifficulty.h"
#include "taskMenu.h"
#include "mapping.h"

Difficulty currentDifficulty = EASY;


void taskDifficulty(void * params){

    ButtonEvent receivedButton;

    int brightness = 0;
    int step = 5;

    while(true){
        if(xQueueReceive(inputQueue, &receivedButton, pdMS_TO_TICKS(20))== pdTRUE){
            if(receivedButton != BTN_GREEN && receivedButton != BTN_WHITE){
                
                for(int i = 0; i < 4; i++){
                    pinMode(indexToLed[i], OUTPUT);
                    digitalWrite(indexToLed[i], LOW);
                }

                switch(receivedButton){
                    case BTN_RED:
                        currentDifficulty = HARD;
                    break;

                    case BTN_YELLOW:
                        currentDifficulty = MEDIUM;
                    break;

                    case BTN_BLUE:
                        currentDifficulty = EASY;
                    break;
                }

                currentGameState = chosenMenuOption;
                vTaskResume(menuOptionToTask[chosenMenuOption]);
                vTaskSuspend(difficultyTaskHandle);
            }else if(receivedButton == BTN_WHITE){
                for(int i = 0; i < 4; i++){
                    analogWrite(indexToLed[i], 0);
                }
                vTaskResume(menuTaskHandle);
                vTaskSuspend(difficultyTaskHandle);
                brightness = 0;
                step = 5;
            }
        }

        brightness += step;

        analogWrite(menuOptionToLed[chosenMenuOption],brightness);
        if(brightness >= 255 || brightness <=0){
            step = -step;
        }
        
    }

}