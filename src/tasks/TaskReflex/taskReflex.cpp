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
   
void playSound(bool isCorrect, int randomLed){
    if(isCorrect){
        digitalWrite(indexToLed[randomLed], LOW);
        tone(BUZZER, 1319);
        vTaskDelay(pdMS_TO_TICKS(100));
        tone(BUZZER, 1568);
        vTaskDelay(pdMS_TO_TICKS(100));
        noTone(BUZZER);
    }else{
        digitalWrite(indexToLed[randomLed], LOW);
        tone(BUZZER, 400);
        vTaskDelay(pdMS_TO_TICKS(100));
        tone(BUZZER, 250);
        vTaskDelay(pdMS_TO_TICKS(100));
        noTone(BUZZER);
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
                        playSound(true,randomLed);
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
                        playSound(false,randomLed);
                        break;
                    }
                }                
            }

            //Just in case of Suspend Task and then come back
            if(shouldRestart){
                break;
            }

            //User didn't give the input in time
            if(millis() - last > 1000 && !playerGuess){
                playSound(false,randomLed);
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