#include <Arduino.h>

#include "taskMenu.h"
#include "taskHandles.h"
#include "queues.h"
#include "states.h"
#include "config.h"
#include "mapping.h"


MenuOption currentMenuOption = SIMONSAYS;
MenuOption lastMenuOption = LEADERBOARD;

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

    // Volta para o LED do menu atual
    digitalWrite(menuOptionToLed[currentMenuOption], HIGH);
}


void taskMenu(void *params){
    ButtonEvent receivedButton;
    

    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(GREEN_LED, LOW);

    digitalWrite(menuOptionToLed[currentMenuOption], HIGH);


    //Suspend games tasks before choosing one
    vTaskSuspend(simonTaskHandle);
    vTaskSuspend(leaderboardTaskHandle);
    vTaskSuspend(reflexTaskHandle);
    vTaskSuspend(stroopTaskHandle);
    vTaskSuspend(difficultyTaskHandle);
    
    pmojiOpening();

    while(true){


        if(xQueueReceive(inputQueue, &receivedButton, portMAX_DELAY)== pdTRUE){
            lastMenuOption = currentMenuOption;
            switch (receivedButton){


                case BTN_RED:
                    currentMenuOption = (MenuOption) ((currentMenuOption - 1 + MENU_OPTIONS_SIZE)%MENU_OPTIONS_SIZE);
                break;
            
                case BTN_YELLOW:
                    currentMenuOption = (MenuOption) ((currentMenuOption + 1)%MENU_OPTIONS_SIZE);
                break;

                case BTN_BLUE:

                    //vTaskResume no de escolher dificuldade
                    //vTaskSuspend a si próprio
                    digitalWrite(menuOptionToLed[currentMenuOption], LOW);
                    if(currentMenuOption != LEADERBOARD){
                        vTaskResume(difficultyTaskHandle);
                    }else{
                        vTaskResume(menuOptionToTask[currentMenuOption]);
                    }
                    vTaskSuspend(menuTaskHandle);
                    

                    //When it returns, the primary state is
                    // CurrentOption -> SIMONSAYS -> RED LED
                    // LastOption -> LEADERBOARD
                    currentMenuOption = SIMONSAYS;
                    lastMenuOption = LEADERBOARD;
                break;


                default:
                break;
            } 


            if(currentMenuOption!= lastMenuOption){
                digitalWrite(menuOptionToLed[currentMenuOption], HIGH);
                digitalWrite(menuOptionToLed[lastMenuOption], LOW);
                tone(BUZZER,ledToBuzzer[currentMenuOption]);
                vTaskDelay(pdMS_TO_TICKS(100));
                noTone(BUZZER);
                Serial.println("Trocou de opção");
            }
        }
        
    }
}