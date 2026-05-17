#include "config.h"

//Association between Button Pin and Button event
const int eventToPin[5] = {RED_BUTTON_PIN,BLUE_BUTTON_PIN,YELLOW_BUTTON_PIN, GREEN_BUTTON_PIN, WHITE_BUTTON_PIN};

//Association between menu option and LED
const int menuOptionToLed[4] = {RED_LED, YELLOW_LED, BLUE_LED, GREEN_LED};

//Association between LED to Buzzer Frequency
const int ledToBuzzer[4] = {659,698,783,880};