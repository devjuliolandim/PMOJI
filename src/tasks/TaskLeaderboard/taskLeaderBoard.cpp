#include <Arduino.h>
#include "storage.h"
#include "states.h"
#include "queues.h"
#include "taskHandles.h"
#include "taskLeaderBoard.h"
#include "taskDifficulty.h"
#include "config.h"
#include "globals.h"

const char* difficulties[3] = {"EASY","MEDIUM","HARD"};
const char* gameNames[3] = {"Simon Says", "Reflexo", "Stroop"};
const char* gameIds[3] = {"simon","reflex","stroop"};


void taskLeaderBoard(void * params){
    
    ButtonEvent receivedButton;
    bool shouldPrint = true;

    int brightness = 0;
    int step = 5;
    
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

        if(xQueueReceive(inputQueue, &receivedButton, pdMS_TO_TICKS(20)) == pdTRUE){
            // Trata o botão Branco: Retorna ao Menu
            if(receivedButton == BTN_WHITE){
                currentGameState = MENU; // Altera o estado para que o display atualize a tela
                vTaskResume(menuTaskHandle);
                analogWrite(GREEN_LED, 0);
                vTaskSuspend(leaderboardTaskHandle);
                shouldPrint = true;
                brightness = 0;
                step = 5;
            }
            // Trata o botão Verde: Alterna a dificuldade exibida na tela (0 -> 1 -> 2 -> 0)
            else if(receivedButton == BTN_GREEN){
                screenDifficulty = (Difficulty)((screenDifficulty + 1) % 3);
            }
        }

        brightness += step;
        
        if(brightness>=255 || brightness<=0){
            step = -step;
        }
        analogWrite(GREEN_LED, brightness);

    }
}