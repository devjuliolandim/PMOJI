#include <Arduino.h>

#include "queues.h"
#include "states.h"
#include "taskDifficulty.h"
#include "taskMenu.h"
#include "mapping.h"

Difficulty currentDifficulty = EASY;


void taskDifficulty(void * params){

    ButtonEvent receivedButton;
    bool isValidInput = true;


    // === DEBUG PURPOSE 
    bool shouldPrint = true;

    while(true){

        if(shouldPrint){
            Serial.println("Escolha a dificuldade: ");
            Serial.println("AZUL - EASY");
            Serial.println("AMARELO - MEDIUM");
            Serial.println("VERMELHO - HARD");
            shouldPrint = false;
        }

        if(xQueueReceive(inputQueue, &receivedButton, portMAX_DELAY)== pdTRUE){
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

                case BTN_WHITE:
                    vTaskResume(menuTaskHandle);
                    vTaskSuspend(difficultyTaskHandle);
                    shouldPrint = true;
                    isValidInput = false;
                break;

                default:
                    isValidInput = false;
                break;
            }

            if(isValidInput){
                //Inicia o jogo
                //Suspende a si proprio
                vTaskResume(menuOptionToTask[currentMenuOption]);
                vTaskSuspend(difficultyTaskHandle);
                shouldPrint = true;
            }else{
                isValidInput = true;
            }

        }
    }

}