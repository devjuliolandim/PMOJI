#include "taskDisplay.h"

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
TelaEstado estadoAtual = TELA_MENU;

unsigned long ultimoDebounceTime = 0;
const unsigned long debounceDelay = 200; 

int opcaoSelecionada = 0; // 0 = INICIAR, 1 = SCORES

void inicializarDisplay() {
    pinMode(TFT_LED, OUTPUT);
    analogWrite(TFT_LED, 128); // Brilho em 50%

    pinMode(TFT_RST, OUTPUT);
    digitalWrite(TFT_RST, LOW);
    delay(100);
    digitalWrite(TFT_RST, HIGH);
    delay(100);

    tft.init(240, 320); 
    tft.invertDisplay(true); 
    tft.setRotation(1); // Modo Paisagem
    tft.fillScreen(~ST77XX_WHITE); 
}

void desenharMenuPrincipal(int itemSelecionado) {
    tft.fillScreen(~ST77XX_WHITE); 

    tft.setTextSize(1);
    tft.setTextColor(~ST77XX_BLACK, ~ST77XX_WHITE); 
    tft.setCursor(230, 10);
    tft.print("BATERIA: 100%");

    tft.setTextSize(3);
    tft.setTextColor(~ST77XX_BLACK, ~ST77XX_WHITE); 
    tft.setCursor(45, 40);
    tft.println("P-MOJI CONSOLE");

    tft.setTextSize(2);

    // Botão Iniciar
    if (itemSelecionado == 0) {
        tft.fillRect(50, 100, 220, 40, ~ST77XX_RED);
        tft.setTextColor(~ST77XX_WHITE, ~ST77XX_RED); 
    } else {
        tft.fillRect(50, 100, 220, 40, ~ST77XX_BLUE);
        tft.setTextColor(~ST77XX_WHITE, ~ST77XX_BLUE);
    }
    tft.setCursor(105, 112);
    tft.print("INICIAR");

    // Botão Scores
    if (itemSelecionado == 1) {
        tft.fillRect(50, 155, 220, 40, ~ST77XX_RED);
        tft.setTextColor(~ST77XX_WHITE, ~ST77XX_RED);
    } else {
        tft.fillRect(50, 155, 220, 40, ~ST77XX_BLUE);
        tft.setTextColor(~ST77XX_WHITE, ~ST77XX_BLUE);
    }
    tft.setCursor(115, 167);
    tft.print("SCORES");

    tft.setTextSize(1);
    tft.setTextColor(~ST77XX_YELLOW, ~ST77XX_WHITE); 
    tft.setCursor(40, 215);
    tft.print("[VERMELHO] Mudar Botao  |  [VERDE] Confirmar");
}

void desenharTelaScores(int getSimonHighScore, int getReflexHighScore) {
    tft.fillScreen(~ST77XX_WHITE); 

    tft.setTextSize(3);
    tft.setTextColor(~ST77XX_BLUE, (uint16_t)~ST77XX_WHITE); 
    tft.setCursor(65, 30);
    tft.println("TOP SCORES");

    tft.setTextSize(2);
    tft.setTextColor(~ST77XX_BLACK, (uint16_t)~ST77XX_WHITE); 
 
    tft.setCursor(60, 90);
    tft.print("1. REFLEXO: ");
    tft.print(getReflexHighScore); 
    tft.print(" ms");

    tft.setCursor(60, 130);
    tft.print("2. SIMON: ");
    tft.print(getSimonHighScore);  

    tft.setTextSize(1);
    tft.setTextColor(~ST77XX_YELLOW, (uint16_t)~ST77XX_WHITE);
    tft.setCursor(75, 200);
    tft.print("[BRANCO] Voltar ao Menu Principal");
}

void desenharTelaJogos(int jogoSelecionado) {
    tft.fillScreen(~ST77XX_WHITE); 

    tft.setTextSize(1);
    tft.setTextColor(~ST77XX_BLACK, (uint16_t)~ST77XX_WHITE);
    tft.setCursor(230, 10);
    tft.print("BATERIA: 100%");

    tft.setTextSize(3);
    tft.setTextColor(~ST77XX_BLUE, (uint16_t)~ST77XX_WHITE); 
    tft.setCursor(45, 30);
    tft.println("SELECIONE O JOGO");

    tft.setTextSize(2);

    // Opção Simon Says
    if (jogoSelecionado == 0) {
        tft.fillRect(40, 85, 240, 35, ~ST77XX_RED); 
        tft.setTextColor(~ST77XX_WHITE, ~ST77XX_RED);
    } else {
        tft.fillRect(40, 85, 240, 35, ~ST77XX_BLUE); 
        tft.setTextColor(~ST77XX_WHITE, ~ST77XX_BLUE);
    }
    tft.setCursor(95, 95);
    tft.print("SIMON SAYS");

    // Opção Reflexo
    if (jogoSelecionado == 1) {
        tft.fillRect(40, 130, 240, 35, ~ST77XX_RED); 
        tft.setTextColor(~ST77XX_WHITE, ~ST77XX_RED);
    } else {
        tft.fillRect(40, 130, 240, 35, ~ST77XX_BLUE); 
        tft.setTextColor(~ST77XX_WHITE, ~ST77XX_BLUE);
    }
    tft.setCursor(115, 140);
    tft.print("REFLEXO");

    // Opção Stroop
    if (jogoSelecionado == 2) {
        tft.fillRect(40, 175, 240, 35, ~ST77XX_RED); 
        tft.setTextColor(~ST77XX_WHITE, ~ST77XX_RED);
    } else {
        tft.fillRect(40, 175, 240, 35, ~ST77XX_BLUE); 
        tft.setTextColor(~ST77XX_WHITE, ~ST77XX_BLUE);
    }
    tft.setCursor(120, 185);
    tft.print("STROOP");

    tft.setTextSize(1);
    tft.setTextColor(~ST77XX_YELLOW, (uint16_t)~ST77XX_WHITE);
    tft.setCursor(25, 220);
    tft.print("[VERMELHO] Mudar Jogo  |  [VERDE] Confirmar");
}

void desenharGameplayReflexo(int scoreAtual) {
    tft.fillScreen(~ST77XX_WHITE); 

    tft.setTextSize(1);
    tft.setTextColor(~ST77XX_BLACK, (uint16_t)~ST77XX_WHITE);
    tft.setCursor(230, 10);
    tft.print("BATERIA: 100%");

    tft.setTextSize(2);
    tft.setTextColor(~ST77XX_YELLOW, (uint16_t)~ST77XX_WHITE);
    tft.setCursor(95, 40);
    tft.print("JOGO REFLEXO");

    tft.setTextSize(2);
    tft.setTextColor(~ST77XX_WHITE, (uint16_t)~ST77XX_WHITE);
    tft.setCursor(110, 100);
    tft.print("PONTOS");

    tft.setTextSize(6);
    tft.setTextColor(~ST77XX_GREEN, (uint16_t)~ST77XX_WHITE);
    
    // Centralização dinâmica do Score
    if (scoreAtual < 10) {
        tft.setCursor(145, 140);
    } else {
        tft.setCursor(130, 140);
    }
    tft.print(scoreAtual);
}

void desenharGameplaySimon(int scoreAtual) {
    tft.fillScreen(~ST77XX_WHITE); 
    tft.setTextSize(1);
    tft.setTextColor(~ST77XX_BLACK, (uint16_t)~ST77XX_WHITE);
    tft.setCursor(230, 10);
    tft.print("BATERIA: 100%");

    tft.setTextSize(2);
    tft.setTextColor(~ST77XX_YELLOW, (uint16_t)~ST77XX_WHITE); 
    tft.setCursor(105, 40);
    tft.print("SIMON SAYS");

    tft.setTextSize(2);
    tft.setTextColor(~ST77XX_WHITE, (uint16_t)~ST77XX_WHITE);
    tft.setCursor(100, 100);
    tft.print("SEQUENCIA");

    tft.setTextSize(6);
    tft.setTextColor(~ST77XX_GREEN, (uint16_t)~ST77XX_WHITE);
    
    // Centralização dinâmica da Sequência
    if (scoreAtual < 10) {
        tft.setCursor(145, 140);
    } else {
        tft.setCursor(130, 140);
    }
    tft.print(scoreAtual);
}

void processarNavegacaoMenu() {
  
}