#include <Arduino.h>

#include "taskMenu.h"
#include "taskHandles.h"
#include "queues.h"
#include "states.h"
#include "config.h"
#include "mapping.h"
#include "output.h"

GameState currentGameState = MENU;
GameState chosenMenuOption = SIMONSAYS;

void taskMenu(void *params){
    ButtonEvent receivedButton;
    
    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(GREEN_LED, LOW);

    vTaskSuspend(simonTaskHandle);
    vTaskSuspend(leaderboardTaskHandle);
    vTaskSuspend(reflexTaskHandle);
    vTaskSuspend(stroopTaskHandle);
    vTaskSuspend(difficultyTaskHandle);
    
    pmojiOpening();
    int brightness = 0;
    int step = 5;
    
    while(true){
        if(xQueueReceive(inputQueue, &receivedButton, pdMS_TO_TICKS(20)) == pdTRUE){
            
            if(receivedButton != BTN_WHITE){

                for(int i = 0; i < 4; i++){
                    analogWrite(indexToLed[i], 0);
                }

                switch (receivedButton){
                    case BTN_RED:
                        chosenMenuOption = SIMONSAYS;
                    break;
                
                    case BTN_YELLOW:
                        chosenMenuOption = STROOP;
                    break;

                    case BTN_BLUE:
                        chosenMenuOption = REFLEX;
                    break;

                    case BTN_GREEN:
                        chosenMenuOption = LEADERBOARD;
                    break;
                }

                if(receivedButton != BTN_GREEN){
                    currentGameState = DIFFICULTY;
                    vTaskResume(difficultyTaskHandle);
                }else{
                    for(int i = 0; i < 4; i++){
                        pinMode(indexToLed[i], OUTPUT);
                        digitalWrite(indexToLed[i], LOW);
                    }

                    currentGameState = LEADERBOARD;
                    vTaskResume(leaderboardTaskHandle);
                }
                
                vTaskSuspend(menuTaskHandle);
                
                brightness = 0;
                step = 5;
            } 
        }

        brightness += step;

        if(brightness >= 255 || brightness <= 0){
            step = -step;
        }

        analogWrite(RED_LED, brightness);
        analogWrite(YELLOW_LED, brightness);
        analogWrite(BLUE_LED, brightness);
        analogWrite(GREEN_LED, brightness);
    }
}