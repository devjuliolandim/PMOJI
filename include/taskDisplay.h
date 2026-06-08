#ifndef TASKDISPLAY_H
#define TASKDISPLAY_H

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include "config.h" 
extern Adafruit_ST7789 tft;

// Protótipos das funções que gerenciam a tela
void inicializarDisplay();
void desenharTelaStatus();

#endif // TASKDISPLAY_H