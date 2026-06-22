#include <Arduino.h>
#include "taskSimonSays.h"
#include "taskMenu.h"
#include "random"
#include "queues.h"
#include "mapping.h"
#include "storage.h"
#include "taskDifficulty.h"
#include "taskInput.h"
#include "globals.h"
#include "output.h"

void taskSimonSays(void * params){

    ButtonEvent receivedButton;

    int sequence[SEQUENCE_SIZE];
    score = 0;

    bool isBegining = true;
    bool isGameOver = false;
    bool isMenuActive = false;

    while(true){
        while(score < SEQUENCE_SIZE && !isMenuActive){
            
            if(isGameOver){

                int highScore = getScore("simon");

                if(score > highScore){

                    Serial.print("NOVO HIGHSCORE: ");
                    Serial.println(score);
                    Serial.print("ANTIGO HIGHSCORE: ");
                    Serial.println(getScore("simon"));
                    highScore = score;
                    saveScore("simon",highScore);
                }

                score = 0;
                isGameOver = false;
                gameOverRoutine();
            }


            if(isBegining){
                isBegining = false;
                gameBeginRoutine();
            }

            sequence[score] = random(0,4);

            int i = 0;


            //Global Variable that blocks the queue from receive inputs
            //The WHITE BUTTON is the only exception
            inputsEnabled = false;



            vTaskDelay(pdMS_TO_TICKS(350));            
            while(i < score + 1 && !isMenuActive){
                blinkAndBuzzer(sequence[i], SEQUENCE_DELAY - 50 * currentDifficulty);
                vTaskDelay(pdMS_TO_TICKS(250 - 50* currentDifficulty));
                i++;
            }

            
            i = 0;

            xQueueReset(inputQueue);
            inputsEnabled = true;

            while(i < score + 1 && !isMenuActive){

                if(xQueueReceive(inputQueue, &receivedButton, portMAX_DELAY)== pdTRUE){

                    Serial.print("Led sorteado : ");
                    Serial.println(sequence[i]);
                    Serial.print("Botão apertado : ");
                    Serial.println(receivedButton);
                    
                  if((int)receivedButton == sequence[i]){
                    blinkAndBuzzer((int)receivedButton, INPUT_DELAY);
                    i++;
                  }else if((int) receivedButton == BTN_WHITE){
                    isMenuActive = true;
                    break;
                }else{
                    isGameOver = true;
                    isBegining = true;
                    break;
                  } 
                }
            }

            if(isMenuActive){
                score = 0;
                isBegining = true;
                isGameOver = false;
                vTaskResume(menuTaskHandle);
                vTaskSuspend(simonTaskHandle);
                isMenuActive = false;
            }else if (!isGameOver){
                score++;
            }
        }
    }

}