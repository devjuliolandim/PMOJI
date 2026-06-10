#include <Arduino.h>
#include "config.h"
#include "queues.h"
#include "states.h"
#include "storage.h"
#include "mapping.h"
#include "taskDisplay.h"
#include "taskInput.h"
#include "taskMenu.h"
#include "taskHandles.h"
#include "taskSimonSays.h"
#include "taskLeaderBoard.h"
#include "taskReflex.h"

QueueHandle_t inputQueue;
TaskHandle_t menuTaskHandle = NULL;
TaskHandle_t simonTaskHandle = NULL;
TaskHandle_t stroopTaskHandle = NULL;
TaskHandle_t reflexTaskHandle = NULL;
TaskHandle_t leaderboardTaskHandle = NULL;

TaskHandle_t menuOptionToTask[4];


void setup() {

    Serial.begin(9600);

    pinMode(RED_LED, OUTPUT);
    pinMode(YELLOW_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);

    // --- INICIALIZAÇÃO DE ENTRADAS (Botões com Pull-up interno) ---
    pinMode(RED_BUTTON_PIN, INPUT_PULLUP);
    pinMode(BLUE_BUTTON_PIN, INPUT_PULLUP);
    pinMode(YELLOW_BUTTON_PIN, INPUT_PULLUP);
    pinMode(GREEN_BUTTON_PIN, INPUT_PULLUP);
    pinMode(WHITE_BUTTON_PIN, INPUT_PULLUP);

    inputQueue = xQueueCreate(10, sizeof(ButtonEvent));

    xTaskCreatePinnedToCore(
        taskInput,
        "Task Input",
        2048,
        NULL,
        1,
        NULL,
        0
    );

    xTaskCreatePinnedToCore(
        taskSimonSays,
        "Simon",
        4096,
        NULL,
        1,
        &simonTaskHandle,
        1
    );

    xTaskCreatePinnedToCore(
        taskReflex,
        "Reflex",
        4096,
        NULL,
        1,
        &reflexTaskHandle,
        1
    );

    xTaskCreatePinnedToCore(
        taskLeaderBoard,
        "LeaderBoard",
        4096,
        NULL,
        1,
        &leaderboardTaskHandle,
        1
    );
    
    xTaskCreatePinnedToCore(
        taskMenu,
        "Task Menu",
        2048,
        NULL,
        1,
        &menuTaskHandle,
        0
    );

    // Mapeamento organizado dos Handles das Tarefas
    menuOptionToTask[0] = simonTaskHandle;
    menuOptionToTask[1] = stroopTaskHandle;
    menuOptionToTask[2] = reflexTaskHandle;
    menuOptionToTask[3] = leaderboardTaskHandle;

    inicializarDisplay();

    estadoAtual = TELA_MENU; // Inicializa em modo Menu
    opcaoSelecionada = 0;    // Foca na primeira opção 
    
    desenharMenuPrincipal(opcaoSelecionada); 

}

void loop() {
    delay(10);
}