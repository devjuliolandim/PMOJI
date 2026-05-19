#include <Arduino.h>
#include "taskReflex.h"
#include "states.h"
#include "mapping.h"
#include "queues.h"
#include "storage.h"


void blinkAndBuzzerReflex(int index, int delay){
        digitalWrite(indexToLed[index], HIGH);
        tone(BUZZER, ledToBuzzer[index]);

        vTaskDelay(pdMS_TO_TICKS(delay));

        digitalWrite(indexToLed[index], LOW);
        noTone(BUZZER);
}

void gameBeginRoutineReflex(){
    for(int i = 0; i < 24; i++){

        int index = (i+2) % 4;

        blinkAndBuzzerReflex(index, BEGINNING_DELAY);
    }
}
   



void taskReflex(void * params){

    ButtonEvent receivedButton;


    int now, last;
    int score = 0;
    int buttonTimesOut = true;

    bool playerGuess = false;
    bool shouldRestart = false;

    while(true){
        
        gameBeginRoutineReflex();

        int gameBegin = millis();
        shouldRestart = false;
        score = 0;
        while(millis() - gameBegin < 10000){

            int randomLed = random(0,4);
            
            digitalWrite(indexToLed[randomLed], HIGH);

            last = millis();
            while(millis()-last<1000){
                                
                if(xQueueReceive(inputQueue, &receivedButton, pdMS_TO_TICKS(10))== pdTRUE){
                    if((int) receivedButton == randomLed){
                        
                        score++;
                        digitalWrite(indexToLed[randomLed], LOW);
                        vTaskDelay(pdMS_TO_TICKS(200));
                        playerGuess = true;
                        break;
                    }else if((ButtonEvent) receivedButton == BTN_WHITE){
                        digitalWrite(indexToLed[randomLed], LOW);
                        vTaskResume(menuTaskHandle);
                        vTaskSuspend(reflexTaskHandle);
                        shouldRestart = true;
                        break;
                    }
                    else{
                        digitalWrite(indexToLed[randomLed], LOW);
                        break;
                    }
                }                
            }

            if(shouldRestart){
                break;
            }

            if(millis() - last > 1000 && !playerGuess){
                digitalWrite(indexToLed[randomLed],LOW);
                vTaskDelay(pdMS_TO_TICKS(200));
            }

            playerGuess = false;
        }

        if(!shouldRestart){
            //GAME OVER ROUTINE
            //MUSIC PLAYS WHILE SHOWING THE SCORE
            //ASK IF THE SCORE IS GREATER THAN HIGHSCORE
            
            if(score > getReflexHighScore()){
                saveReflexHighScore(score);
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