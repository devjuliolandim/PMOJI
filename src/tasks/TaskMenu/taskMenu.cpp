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
    
    MenuOption currentMenuOption = SIMONSAYS;
    MenuOption lastMenuOption = LEADERBOARD;

    int jogoSelecionado = 0; // 0 = Simon Says, 1 = Reflexo, 2 = Stroop
    int ultimoJogoSelecionado = -1;

    // Garante leds apagados no início
    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(GREEN_LED, LOW);

    // Liga o LED indicador da opção inicial do menu principal
    digitalWrite(menuOptionToLed[currentMenuOption], HIGH);

    // Susbende as tarefas de jogo no boot inicial do console
    vTaskSuspend(simonTaskHandle);
    vTaskSuspend(leaderboardTaskHandle);
    vTaskSuspend(reflexTaskHandle);
    
    while(true){

        if(xQueueReceive(inputQueue, &receivedButton, portMAX_DELAY) == pdTRUE){
            
            if (estadoAtual == TELA_MENU) {
                lastMenuOption = currentMenuOption;
                
                switch (receivedButton){
                    case BTN_RED: // Alterna para cima / botão anterior
                        currentMenuOption = (MenuOption) ((currentMenuOption - 1 + MENU_OPTIONS_SIZE)%MENU_OPTIONS_SIZE);
                    break;
                
                    case BTN_YELLOW: // Alterna para baixo / próximo botão
                        currentMenuOption = (MenuOption) ((currentMenuOption + 1)%MENU_OPTIONS_SIZE);
                    break;

                    case BTN_GREEN: // Botão de Confirmação (Verde)
                        if (currentMenuOption == MenuOption::SIMONSAYS) { 
                            // Se selecionou INICIAR, joga o console para a Tela de Seleção de Jogos
                            estadoAtual = TELA_JOGOS;
                            jogoSelecionado = 0; // Inicia focando no Simon Says
                            desenharTelaJogos(jogoSelecionado);
                        } else if (currentMenuOption == MenuOption::LEADERBOARD) {
                            // Se selecionou SCORES, vai direto para a task de Records
                            vTaskResume(leaderboardTaskHandle);
                            vTaskSuspend(menuTaskHandle);
                        }
                    break;

                    default:
                    break;
                } 

                // Feedback sonoro e visual de navegação no Menu Principal
                if(currentMenuOption != lastMenuOption){
                    digitalWrite(menuOptionToLed[currentMenuOption], HIGH);
                    digitalWrite(menuOptionToLed[lastMenuOption], LOW);
                    tone(BUZZER, ledToBuzzer[currentMenuOption]);
                    vTaskDelay(pdMS_TO_TICKS(100));
                    noTone(BUZZER);
                }
            }
            
            else if (estadoAtual == TELA_JOGOS) {
                ultimoJogoSelecionado = jogoSelecionado;

                switch (receivedButton) {
                    case BTN_RED: // Botão Vermelho alterna ciclicamente entre as 3 opções (0, 1, 2)
                        jogoSelecionado = (jogoSelecionado + 1) % 3;
                        desenharTelaJogos(jogoSelecionado);
                    break;

                    case BTN_GREEN: // Botão Verde confirma o lançamento do jogo ativo
                        if (jogoSelecionado == 0) {
                            // Acorda a tarefa do Simon Says e pausa o menu
                            vTaskResume(simonTaskHandle);
                            vTaskSuspend(menuTaskHandle);
                            
                            // Reseta o estado para quando o jogo terminar e voltar
                            estadoAtual = TELA_MENU;
                            currentMenuOption = SIMONSAYS;
                        } 
                        else if (jogoSelecionado == 1) {
                            // Acorda a tarefa do Jogo de Reflexo e pausa o menu
                            vTaskResume(reflexTaskHandle);
                            vTaskSuspend(menuTaskHandle);
                            
                            // Reseta o estado para quando o jogo terminar e voltar
                            estadoAtual = TELA_MENU;
                            currentMenuOption = SIMONSAYS;
                        }
                        else if (jogoSelecionado == 2) {
                            // Opção STROOP: Como não vai para lugar nenhum por enquanto, 
                            // apenas emite um bipe curto de ação indisponível
                            tone(BUZZER, 220);
                            vTaskDelay(pdMS_TO_TICKS(150));
                            noTone(BUZZER);
                        }
                    break;

                    case BTN_WHITE: // Botão Branco funciona como um botão de voltar/cancelar
                        estadoAtual = TELA_MENU;
                        desenharMenuPrincipal(opcaoSelecionada);
                        
                        // Restaura o LED indicador do menu principal
                        digitalWrite(menuOptionToLed[currentMenuOption], HIGH);
                    break;

                    default:
                    break;
                }

                // Feedback sonoro de navegação entre as opções da tela de jogos
                if (jogoSelecionado != ultimoJogoSelecionado && ultimoJogoSelecionado != -1) {
                    tone(BUZZER, 880);
                    vTaskDelay(pdMS_TO_TICKS(50));
                    noTone(BUZZER);
                }
            }
        }
    }
}