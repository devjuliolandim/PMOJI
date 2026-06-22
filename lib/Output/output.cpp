#include "output.h"
#include "../../include/config.h"
#include "../../include/mapping.h"
#include "../../include/states.h"
#include "../../include/taskMenu.h"

void blinkAndBuzzer(int index, int delay){
    digitalWrite(indexToLed[index], HIGH);
    tone(BUZZER, ledToBuzzer[index]);

    vTaskDelay(pdMS_TO_TICKS(delay));

    digitalWrite(indexToLed[index], LOW);
    noTone(BUZZER);
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

void gameBeginRoutine(){
    switch (currentMenuOption)
    {
    case SIMONSAYS:
        for(int i = 0; i < 12; i++){

            int index = i % 4;

            blinkAndBuzzer(index, GAME_BEGIN_DELAY);
        }
        break;
    
    case STROOP:
        for(int i = 12; i > 0; i--){

            int index = i % 4;

            blinkAndBuzzer(index, GAME_BEGIN_DELAY);
        }
        break;

    case REFLEX:
    for(int i = 0; i < 1; i++){
            for(int j = 0; j < 2;j++){
                digitalWrite(RED_LED, HIGH);
                digitalWrite(BLUE_LED, HIGH);
                tone(BUZZER, ledToBuzzer[j]);
                vTaskDelay(GAME_BEGIN_DELAY);
                digitalWrite(RED_LED, LOW);
                digitalWrite(BLUE_LED, LOW);
                vTaskDelay(GAME_BEGIN_DELAY);
            }

            for(int j = 0; j < 2;j++){
                digitalWrite(YELLOW_LED, HIGH);
                digitalWrite(GREEN_LED, HIGH);
                tone(BUZZER, ledToBuzzer[j]);
                vTaskDelay(GAME_BEGIN_DELAY);
                digitalWrite(YELLOW_LED, LOW);
                digitalWrite(GREEN_LED, LOW);
                vTaskDelay(GAME_BEGIN_DELAY);
            }

        }
        noTone(BUZZER);

        break;
    }
}

void handleCorrectInputSound(bool isCorrect, int led){
    if(isCorrect){
        digitalWrite(led, LOW);
        tone(BUZZER, 1319);
        vTaskDelay(pdMS_TO_TICKS(100));
        tone(BUZZER, 1568);
        vTaskDelay(pdMS_TO_TICKS(100));
        noTone(BUZZER);
    }else{
        digitalWrite(led, LOW);
        tone(BUZZER, 400);
        vTaskDelay(pdMS_TO_TICKS(100));
        tone(BUZZER, 250);
        vTaskDelay(pdMS_TO_TICKS(100));
        noTone(BUZZER);
    }
}