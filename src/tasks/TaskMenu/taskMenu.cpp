#include <Arduino.h>

#include "taskMenu.h"
#include "queues.h"
#include "states.h"
#include "config.h"
#include "globals.h"

void taskMenu(void *params){
    ButtonEvent receivedButton;
    bool isMenuActive = true;
    MenuOption currentMenuOption = SIMONSAYS;
    MenuOption lastMenuOption = LEADERBOARD;

    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(GREEN_LED, LOW);

    digitalWrite(menuOptionToLed[currentMenuOption], HIGH);


    while(true){

        if(!isMenuActive){
            continue;
        }


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
                    isMenuActive = false;
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
            }
        }
        
    }
}