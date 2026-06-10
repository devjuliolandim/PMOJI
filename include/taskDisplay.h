#ifndef TASKDISPLAY_H
#define TASKDISPLAY_H

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include "config.h"

enum TelaEstado {
    TELA_MENU,
    TELA_JOGOS,
    TELA_SCORES,
    TELA_GAMEPLAY_REFLEXO,
    TELA_GAMEPLAY_SIMON
};

extern Adafruit_ST7789 tft;
extern TelaEstado estadoAtual;
extern int opcaoSelecionada;

void inicializarDisplay();
void desenharMenuPrincipal(int itemSelecionado);
void desenharTelaJogos(int jogoSelecionado);
void desenharTelaScores(int getSimonHighScore, int getReflexHighScore);
void desenharGameplayReflexo(int scoreAtual);
void desenharGameplaySimon(int scoreAtual);
void processarNavegacaoMenu();

#endif 