#include "config.h"
#include "taskHandles.h"

//MAPPING

//The most basic num to LED
const int indexToLed[4] = {RED_LED, YELLOW_LED, BLUE_LED, GREEN_LED};

//Association between Button Pin to Button event
const int eventToPin[5] = {RED_BUTTON_PIN,YELLOW_BUTTON_PIN,BLUE_BUTTON_PIN, GREEN_BUTTON_PIN, WHITE_BUTTON_PIN};

//Association between menu option to LED
const int menuOptionToLed[4] = {RED_LED, YELLOW_LED, BLUE_LED, GREEN_LED};

//Association between LED to Buzzer Frequency
const int ledToBuzzer[4] = {261,392,587,880};

//Association between MenuOption to TaskHandle_t
extern TaskHandle_t menuOptionToTask[4];