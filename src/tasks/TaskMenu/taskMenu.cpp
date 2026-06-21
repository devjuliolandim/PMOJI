#include <Arduino.h>

#include "taskMenu.h"
#include "taskHandles.h"
#include "queues.h"
#include "states.h"
#include "config.h"
#include "mapping.h"

MenuOption currentMenuOption = SIMONSAYS;

void pmojiOpening(){

    struct Note {
        int led;
        int freq;
        int duration;
    };

    Note intro[] = {
        {RED_LED,    523, 120}, // C5
        {YELLOW_LED, 659, 120}, // E5
        {BLUE_LED,   784, 120}, // G5
        {GREEN_LED, 1047, 250}, // C6
    };

    // Garantir que tudo está apagado
    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(GREEN_LED, LOW);

    vTaskDelay(pdMS_TO_TICKS(200));

    // Arpejo de abertura
    for(auto &n : intro){

        digitalWrite(n.led, HIGH);
        tone(BUZZER, n.freq);

        vTaskDelay(pdMS_TO_TICKS(n.duration));

        noTone(BUZZER);
        digitalWrite(n.led, LOW);

        vTaskDelay(pdMS_TO_TICKS(30));
    }

    // Pequena pausa dramática
    vTaskDelay(pdMS_TO_TICKS(100));

    // "Brilho" final
    digitalWrite(RED_LED, HIGH);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(BLUE_LED, HIGH);
    digitalWrite(GREEN_LED, HIGH);

    tone(BUZZER, 1319); // E6
    vTaskDelay(pdMS_TO_TICKS(120));

    tone(BUZZER, 1568); // G6
    vTaskDelay(pdMS_TO_TICKS(120));

    tone(BUZZER, 2093); // C7
    vTaskDelay(pdMS_TO_TICKS(250));

    noTone(BUZZER);

    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(GREEN_LED, LOW);

    vTaskDelay(pdMS_TO_TICKS(100));

}


void taskMenu(void *params){
    ButtonEvent receivedButton;
    

    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(GREEN_LED, LOW);

    //Suspend games tasks before choosing one
    vTaskSuspend(simonTaskHandle);
    vTaskSuspend(leaderboardTaskHandle);
    vTaskSuspend(reflexTaskHandle);
    vTaskSuspend(stroopTaskHandle);
    vTaskSuspend(difficultyTaskHandle);
    
    pmojiOpening();
    int brightness = 0;
    int step = 5;
    
    while(true){
        if(xQueueReceive(inputQueue, &receivedButton, pdMS_TO_TICKS(20))== pdTRUE){
            
            if(receivedButton != BTN_WHITE){

                for(int i = 0; i < 4; i++){
                    analogWrite(indexToLed[i], 0);
                }

                switch (receivedButton){
                    case BTN_RED:
                        currentMenuOption = SIMONSAYS;
                    break;
                
                    case BTN_YELLOW:
                        currentMenuOption = STROOP;
                    break;

                    case BTN_BLUE:
                        currentMenuOption = REFLEX;
                    break;

                    case BTN_GREEN:
                        currentMenuOption = LEADERBOARD;
                    break;
                }
                if(receivedButton != BTN_GREEN){
                    vTaskResume(difficultyTaskHandle);
                }else{
                    for(int i = 0; i < 4; i++){
                        pinMode(indexToLed[i], OUTPUT);
                        digitalWrite(indexToLed[i], LOW);
                    }
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