#include <Arduino.h>

#include "taskMenu.h"
#include "taskHandles.h"
#include "queues.h"
#include "states.h"
#include "config.h"
#include "mapping.h"

void taskMenu(void *params){
    ButtonEvent receivedButton;
    MenuOption currentMenuOption = SIMONSAYS;
    MenuOption lastMenuOption = LEADERBOARD;

    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(GREEN_LED, LOW);

    digitalWrite(menuOptionToLed[currentMenuOption], HIGH);


    //Suspend games tasks before choosing one
    vTaskSuspend(simonTaskHandle);
    vTaskSuspend(leaderboardTaskHandle);
    
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
                    vTaskResume(menuOptionToTask[currentMenuOption]);
                    vTaskSuspend(menuTaskHandle);
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
                Serial.println("Hi");

            }
        }
        
    }
}