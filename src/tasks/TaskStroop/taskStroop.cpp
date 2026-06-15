#include <Arduino.h>
#include "taskStroop.h"
#include "states.h"
#include "mapping.h"
#include "queues.h"
#include "storage.h"
#include <vector>
#include <string>
#include <algorithm>
#include <random> 

std::vector<String> words = {"Vermelho", "Amarelo", "Azul", "Verde"};
std::vector<int> nums = {0,1,2,3};

void taskStroop(void * params){

    ButtonEvent receivedButton;

    int now, last;
    int score = 0;
    int buttonTimesOut = true;

    bool playerGuess = false;
    bool shouldRestart = false;

    std::random_device rd;

    std::mt19937 g(rd());

    while(true){
        
       // gameBeginRoutineReflex();
       // change for stroop

        int gameBegin = millis();
        shouldRestart = false;
        score = 0;
        while(millis() - gameBegin < GAME_TIME){

            //Embaralha
            std::shuffle(nums.begin(),nums.end(), g);

            //Escolhe o primeiro de index (Cor Sorteada)
            int randomLed = nums[0];

            Serial.print("Palavra Sorteada:");
            Serial.println(words[nums[1]]);
            Serial.print("Cor Sorteada: ");
            Serial.println(randomLed);

            digitalWrite(indexToLed[randomLed], HIGH);
        

            last = millis();
            while(millis()-last<RESPONSE_TIME){
                                
                if(xQueueReceive(inputQueue, &receivedButton, pdMS_TO_TICKS(10))== pdTRUE){
                    if((int) receivedButton == nums[1]){
                        
                        score++;
                        //change for stroop
                        //playSound(true,randomLed);
                        digitalWrite(indexToLed[randomLed],LOW);
                        vTaskDelay(pdMS_TO_TICKS(100));
                        playerGuess = true;
                        break;
                    }else if((ButtonEvent) receivedButton == BTN_WHITE){
                        digitalWrite(indexToLed[randomLed], LOW);
                        vTaskResume(menuTaskHandle);
                        vTaskSuspend(stroopTaskHandle);
                        shouldRestart = true;
                        break;
                    }
                    else{
                        //change for stroop
                        //playSound(false,randomLed);
                        digitalWrite(indexToLed[randomLed],LOW);
                        vTaskDelay(pdMS_TO_TICKS(100));
                        break;
                    }
                    
                }                
            }

            //Just in case of Suspend Task and then come back
            if(shouldRestart){
                break;
            }

            //User didn't give the input in time
            if(!playerGuess){
                //change for stroop
                //playSound(false,randomLed);
                digitalWrite(indexToLed[randomLed],LOW);
                vTaskDelay(pdMS_TO_TICKS(100));
            }

            playerGuess = false;
        }

        if(!shouldRestart){
            //GAME OVER ROUTINE
            //MUSIC PLAYS WHILE SHOWING THE SCORE
            //ASK IF THE SCORE IS GREATER THAN HIGHSCORE
            
            if(score > getScore("stroop")){
                saveScore("stroop",score);
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