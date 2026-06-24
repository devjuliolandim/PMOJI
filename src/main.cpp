#include <Arduino.h>
#include "config.h"
#include "queues.h"
#include "states.h"
#include "storage.h"
#include "mapping.h"
#include "taskInput.h"
#include "taskMenu.h"
#include "taskHandles.h"
#include "taskSimonSays.h"
#include "taskLeaderBoard.h"
#include "taskReflex.h"
#include "taskStroop.h"
#include "taskDifficulty.h"
#include "taskDisplay.h"

QueueHandle_t inputQueue;
TaskHandle_t menuTaskHandle = NULL;
TaskHandle_t simonTaskHandle = NULL;
TaskHandle_t stroopTaskHandle = NULL;
TaskHandle_t reflexTaskHandle = NULL;
TaskHandle_t leaderboardTaskHandle = NULL;
TaskHandle_t difficultyTaskHandle = NULL;
TaskHandle_t displayTaskHandle = NULL;

TaskHandle_t menuOptionToTask[4];


void setup() {

    Serial.begin(9600);

    pinMode(RED_LED, OUTPUT);
    pinMode(YELLOW_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);

    pinMode(BUZZER, OUTPUT);

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
        taskStroop,
        "Stroop",
        8192,
        NULL,
        1,
        &stroopTaskHandle,
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
        taskDifficulty,
        "taskDifficulty",
        2048,
        NULL,
        1,
        &difficultyTaskHandle,
        0
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

    xTaskCreatePinnedToCore(
        taskDisplay,
        "Task Display",
        4096,          // Alocação segura para pilhas gráficas da Adafruit
        NULL,
        1,             // Prioridade balanceada com as demais tasks de interface
        &displayTaskHandle,
        1              // Executa no Core 1 (deixando o Core 0 livre para inputs e cálculos de bateria)
    );

    menuOptionToTask[0] = simonTaskHandle;
    menuOptionToTask[1] = stroopTaskHandle;
    menuOptionToTask[2] = reflexTaskHandle;
    menuOptionToTask[3] = leaderboardTaskHandle;
    vTaskSuspend(difficultyTaskHandle);
    vTaskSuspend(reflexTaskHandle);
}

void loop() {

}