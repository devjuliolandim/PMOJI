#include<Arduino.h>

#include "config.h"
#include "states.h"
#include "queues.h"
#include "mapping.h"
#include "taskInput.h"


bool inputsEnabled = true;


//DEBUG PURPOSE ONLY
void printButton(ButtonEvent btn){
    switch(btn){
        case BTN_RED:
            Serial.println("Botão Vermelho Apertado!");
        break;

        case BTN_YELLOW:
            Serial.println("Botão Amarelo Apertado!");
        break;

        case BTN_BLUE:
            Serial.println("Botão Azul Apertado!");
        break;
        
        case BTN_GREEN:
            Serial.println("Botão Verde Apertado!");
        break;
    }
}

void taskInput(void *params){
    ButtonEvent event;

    pinMode(RED_BUTTON_PIN, INPUT_PULLUP);
    pinMode(BLUE_BUTTON_PIN, INPUT_PULLUP);
    pinMode(YELLOW_BUTTON_PIN, INPUT_PULLUP);
    pinMode(GREEN_BUTTON_PIN, INPUT_PULLUP);
    pinMode(WHITE_BUTTON_PIN, INPUT_PULLUP);
    

    while(true){
        event = ButtonEvent::BTN_NONE;
        
        for(int i = 0; i < 5; i++){
            if(digitalRead(eventToPin[i])== LOW){
                event = (ButtonEvent)i;
                break;
            }
        }

        if(event == BTN_WHITE){
            Serial.println("BOTÃO BRANCO APERTADO!!!!!!");
        }

        if(event != BTN_NONE){

            if(event == BTN_WHITE || inputsEnabled){

                xQueueSend(inputQueue,&event, portMAX_DELAY);

                //Debounce
                vTaskDelay(pdMS_TO_TICKS(200));
            }
        }

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}
