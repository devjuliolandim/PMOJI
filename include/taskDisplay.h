#ifndef TASKDISPLAY_H
#define TASKDISPLAY_H

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include "config.h"

enum TelaEstado {
    TELA_MENU,
    TELA_SCORES
};

extern Adafruit_ST7789 tft;
extern TelaEstado estadoAtual;
extern int opcaoSelecionada;

void inicializarDisplay();
void desenharMenuPrincipal(int itemSelecionado);
void desenharTelaScores(int getSimonHighScore, int getReflexHighScore);
void processarNavegacaoMenu();

#endif 