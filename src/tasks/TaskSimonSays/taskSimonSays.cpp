#include <Arduino.h>
#include "taskSimonSays.h"
#include "taskMenu.h"
#include "random"
#include "queues.h"
#include "mapping.h"
#include "globals.h"
#include "storage.h"

void blinkAndBuzzer(int index, int delay){
        digitalWrite(indexToLed[index], HIGH);
        tone(BUZZER, ledToBuzzer[index]);

        vTaskDelay(pdMS_TO_TICKS(delay));

        digitalWrite(indexToLed[index], LOW);
        noTone(BUZZER);
}

void gameBeginRoutine(){
    for(int i = 0; i < 12; i++){

        int index = i % 4;

        blinkAndBuzzer(index, BEGINNING_DELAY);
    }
}

void gameOverRoutine(){


    const int gameOverNotes[4] = {523,392,262,130};


    for(int i = 0; i < 4 ;i++){
        digitalWrite(RED_LED,HIGH);
        digitalWrite(YELLOW_LED,HIGH);
        digitalWrite(BLUE_LED,HIGH);
        digitalWrite(GREEN_LED,HIGH);
        tone(BUZZER, gameOverNotes[i]);
        vTaskDelay(pdMS_TO_TICKS(100));
        digitalWrite(RED_LED,LOW);
        digitalWrite(YELLOW_LED,LOW);
        digitalWrite(BLUE_LED,LOW);
        digitalWrite(GREEN_LED,LOW);
        vTaskDelay(pdMS_TO_TICKS(100));
        noTone(BUZZER);

    }
}

void taskSimonSays(void * params){

    ButtonEvent receivedButton;

    int sequence[SEQUENCE_SIZE];
    int counter = 0;

    bool isBegining = true;
    bool isGameOver = false;
    bool isMenuActive = false;

    while(true){
        while(counter < SEQUENCE_SIZE && !isMenuActive){
            if(isBegining){
                isBegining = false;
                gameBeginRoutine();
            }

            if(isGameOver){

                int highScore = getSimonHighScore();

                if(counter > highScore){

                    Serial.print("NOVO HIGHSCORE: ");
                    Serial.println(counter);
                    Serial.print("ANTIGO HIGHSCORE: ");
                    Serial.println(getSimonHighScore());
                    highScore = counter;
                    saveSimonHighScore(highScore);
                }

                counter = 0;
                isGameOver = false;
                gameOverRoutine();
            }

            sequence[counter] = random(0,4);

            int i = 0;
            inputsEnabled = false;

            vTaskDelay(pdMS_TO_TICKS(250));            
            while(i < counter + 1 && !isMenuActive){
                blinkAndBuzzer(sequence[i], SEQUENCE_DELAY);
                vTaskDelay(pdMS_TO_TICKS(250));
                i++;
            }

            
            i = 0;

            xQueueReset(inputQueue);
            inputsEnabled = true;

            while(i < counter + 1 && !isMenuActive){

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
                counter = 0;
                isBegining = true;
                isGameOver = false;
                vTaskResume(menuTaskHandle);
                vTaskSuspend(simonTaskHandle);
                isMenuActive = false;
            }else if (!isGameOver){
                counter++;
            }
        }
    }

}