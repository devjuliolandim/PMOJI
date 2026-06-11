#include <Arduino.h>
#include "taskMenu.h"
#include "taskHandles.h"
#include "queues.h"
#include "states.h"
#include "config.h"
#include "mapping.h"
#include "taskDisplay.h"

void taskMenu(void *params){
    ButtonEvent receivedButton;
    
    int currentMenuOption = 0; // 0 = INICIAR, 1 = SCORES
    int lastMenuOption = 1;

    int jogoSelecionado = 0; // 0 = SIMON, 1 = REFLEXO, 2 = STROOP
    int ultimoJogoSelecionado = -1;

    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(GREEN_LED, LOW);

    digitalWrite(menuOptionToLed[0], HIGH);

    vTaskSuspend(simonTaskHandle);
    vTaskSuspend(leaderboardTaskHandle);
    vTaskSuspend(reflexTaskHandle);
    
    while(true){
        if(xQueueReceive(inputQueue, &receivedButton, portMAX_DELAY) == pdTRUE){
            
            // Fluxo do Menu Principal
            if (estadoAtual == TELA_MENU) {
                lastMenuOption = currentMenuOption;
                
                switch (receivedButton){
                    case BTN_RED: // Sobe na lista
                        currentMenuOption = (currentMenuOption - 1 + 2) % 2; 
                    break;
                
                    case BTN_YELLOW: // Desce na lista
                        currentMenuOption = (currentMenuOption + 1) % 2;
                    break;

                    case BTN_GREEN: // Confirmar entrada
                        if (currentMenuOption == 0) { 
                            estadoAtual = TELA_JOGOS;
                            jogoSelecionado = 0; 
                            desenharTelaJogos(jogoSelecionado);
                        } 
                        else if (currentMenuOption == 1) {
                            estadoAtual = TELA_SCORES;
                            vTaskResume(leaderboardTaskHandle);
                            vTaskSuspend(menuTaskHandle);
                        }
                    break;

                    default:
                    break;
                } 

                // Atualizacao do hardware e display do Menu Principal
                if(currentMenuOption != lastMenuOption){
                    digitalWrite(menuOptionToLed[currentMenuOption], HIGH);
                    digitalWrite(menuOptionToLed[lastMenuOption], LOW);
                    
                    tone(BUZZER, ledToBuzzer[currentMenuOption]);
                    vTaskDelay(pdMS_TO_TICKS(100));
                    noTone(BUZZER);

                    desenharMenuPrincipal(currentMenuOption);
                }
            }
            
            // Fluxo da Selecao de Jogos
            else if (estadoAtual == TELA_JOGOS) {
                ultimoJogoSelecionado = jogoSelecionado;

                switch (receivedButton) {
                    case BTN_RED: // Avança na lista
                        jogoSelecionado = (jogoSelecionado + 1) % 3;
                        desenharTelaJogos(jogoSelecionado);
                    break;

                    case BTN_YELLOW: // Retorna na lista
                        jogoSelecionado = (jogoSelecionado - 1 + 3) % 3;
                        desenharTelaJogos(jogoSelecionado);
                    break;

                    case BTN_GREEN: // Inicia o jogo focado
                        if (jogoSelecionado == 0) {
                            vTaskResume(simonTaskHandle);
                            vTaskSuspend(menuTaskHandle);
                            estadoAtual = TELA_MENU;
                            currentMenuOption = 0;
                        } 
                        else if (jogoSelecionado == 1) {
                            vTaskResume(reflexTaskHandle);
                            vTaskSuspend(menuTaskHandle);
                            estadoAtual = TELA_MENU;
                            currentMenuOption = 0;
                        }
                        else if (jogoSelecionado == 2) {
                            // Stroop indisponível
                            tone(BUZZER, 220);
                            vTaskDelay(pdMS_TO_TICKS(150));
                            noTone(BUZZER);
                        }
                    break;

                    case BTN_WHITE: // Voltar ao Menu Principal
                        estadoAtual = TELA_MENU;
                        currentMenuOption = 0; 
                        desenharMenuPrincipal(currentMenuOption);
                        digitalWrite(menuOptionToLed[currentMenuOption], HIGH);
                    break;

                    default:
                    break;
                }

                // Som de clique ao mudar selecao
                if (jogoSelecionado != ultimoJogoSelecionado && ultimoJogoSelecionado != -1) {
                    tone(BUZZER, 880); 
                    vTaskDelay(pdMS_TO_TICKS(50));
                    noTone(BUZZER);
                }
            }
        }
    }
}