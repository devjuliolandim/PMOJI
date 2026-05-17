#include <Arduino.h>

#include "queues.h"
#include "states.h"
#include "taskInput.h"

QueueHandle_t inputQueue;

void setup() {

    Serial.begin(9600);

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
}

void loop() {

}