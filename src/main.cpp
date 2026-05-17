#include <Arduino.h>
#include "config.h"
#include "queues.h"
#include "states.h"
#include "taskInput.h"
#include "taskMenu.h"

QueueHandle_t inputQueue;

void setup() {

    Serial.begin(9600);

    pinMode(RED_LED, OUTPUT);
    pinMode(YELLOW_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);

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
        taskMenu,
        "Task Menu",
        2048,
        NULL,
        1,
        NULL,
        0
    );
}

void loop() {

}