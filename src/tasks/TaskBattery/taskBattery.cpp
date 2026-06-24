#include <Arduino.h>
#include "taskBattery.h"
#include "config.h"

float batteryLevel = 0.0f;

void taskBattery(void *params)
{
    batteryLevel = 0;
    
    while (true)
    {
        int adc = analogRead(BATTERY_PIN);

        batteryLevel = (adc / 4095.0) * 3.3 * 2.0;

        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}