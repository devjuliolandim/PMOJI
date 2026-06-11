#include <Arduino.h>
#include "storage.h"
#include "states.h"
#include "queues.h"
#include "taskHandles.h"
#include "taskLeaderBoard.h"
#include "taskDisplay.h"
#include "mapping.h"

void taskLeaderBoard(void *params) {
    ButtonEvent receivedButton;
    bool shouldPrint = true;

    while(true) {
        if(shouldPrint) {
            Serial.println("RECORDES: ");   
            Serial.print("Simon Says: ");
            Serial.println(getScore("simon"));
            Serial.print("Reflex: ");
            Serial.println(getScore("reflex"));

            estadoAtual = TELA_SCORES;
            desenharTelaScores(getScore("simon"), getScore("reflex"));
            
            shouldPrint = false; // Garante renderizacao unica
        }

        if(xQueueReceive(inputQueue, &receivedButton, pdMS_TO_TICKS(50)) == pdTRUE) {
            
            if(receivedButton == BTN_WHITE) {
                Serial.println("[Leaderboard] Botão Branco pressionado! Voltando ao Menu...");
                
                shouldPrint = true; 
                estadoAtual = TELA_MENU;
                desenharMenuPrincipal(1); 
                
                digitalWrite(menuOptionToLed[1], HIGH);
                
                vTaskResume(menuTaskHandle);
                vTaskSuspend(leaderboardTaskHandle); // Pausa a tarefa atual
            }
        }

        vTaskDelay(pdMS_TO_TICKS(20)); // Alimenta o Watchdog
    }
}