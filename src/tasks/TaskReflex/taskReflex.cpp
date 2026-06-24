#include <Arduino.h>
#include "taskReflex.h"
#include "states.h"
#include "mapping.h"
#include "queues.h"
#include "storage.h"
#include "taskDifficulty.h"
#include "taskInput.h"
#include "globals.h"
#include "output.h"
   
void taskReflex(void * params){

    ButtonEvent receivedButton;

    int now, last;
    score = 0;
    int buttonTimesOut = true;

    bool playerGuess = false;
    bool shouldRestart = false;

    while(true){
    
        inputsEnabled = false;
        gameBeginRoutine();
        xQueueReset(inputQueue);
        inputsEnabled = true;

        int gameBegin = millis();
        shouldRestart = false;
        score = 0;
        while(millis() - gameBegin < GAME_TIME){

            int randomLed = random(0,4);
            
            digitalWrite(indexToLed[randomLed], HIGH);

            last = millis();

            xQueueReset(inputQueue);
            while(millis()-last<(RESPONSE_TIME - 300 * currentDifficulty)){
                                
                if(xQueueReceive(inputQueue, &receivedButton, pdMS_TO_TICKS(10))== pdTRUE){
                    if((int) receivedButton == randomLed){
                        score++;
                        handleCorrectInputSound(true,indexToLed[randomLed]);
                        playerGuess = true;
                        break;
                    }else if((ButtonEvent) receivedButton == BTN_WHITE){
                        digitalWrite(indexToLed[randomLed], LOW);
                        score = 0;
                        currentGameState = MENU;
                        shouldRestart = true;
                        vTaskResume(menuTaskHandle);
                        vTaskSuspend(reflexTaskHandle);
                        break;
                    }
                    else{
                        handleCorrectInputSound(false,indexToLed[randomLed]);
                        break;
                    }
                }                
            }

            if(shouldRestart){
                break;
            }

            if(millis()-last>=RESPONSE_TIME - 300 * currentDifficulty && !playerGuess){
                handleCorrectInputSound(false,indexToLed[randomLed]);
            }

            playerGuess = false;
        }

        if(!shouldRestart){
            if(score > getScore("reflex")){
                saveScore("reflex",score);
                Serial.println("PARABÉNS VOCÊ BATEU O NOVO RECORDE!!!!");
                Serial.print(score);
                Serial.println(" pts");
            }else{
                Serial.print("Sua pontução foi de ");
                Serial.print(score);
                Serial.println(" pts");
            }
        }
    }
}