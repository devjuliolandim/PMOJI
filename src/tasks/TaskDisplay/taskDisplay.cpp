#include "taskDisplay.h"

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
TelaEstado estadoAtual = TELA_MENU;

unsigned long ultimoDebounceTime = 0;
const unsigned long debounceDelay = 200; 

int opcaoSelecionada = 0; // 0 = INICIAR, 1 = SCORES

void inicializarDisplay() {
    pinMode(TFT_LED, OUTPUT);
    analogWrite(TFT_LED, 128); 

    pinMode(TFT_RST, OUTPUT);
    digitalWrite(TFT_RST, LOW);
    delay(100);
    digitalWrite(TFT_RST, HIGH);
    delay(100);

    tft.init(240, 320); 
    tft.invertDisplay(true); 
    tft.setRotation(1); // Modo Paisagem (320x240)
    tft.fillScreen(~ST77XX_WHITE); // Fundo Preto Real
}

void desenharMenuPrincipal(int itemSelecionado) {
    tft.fillScreen(~ST77XX_WHITE); // Garante fundo Preto

    // --- INDICADOR DE BATERIA ---
    tft.setTextSize(1);
    tft.setTextColor(~ST77XX_BLACK, ~ST77XX_WHITE); // Texto Branco no fundo Preto
    tft.setCursor(230, 10);
    tft.print("BATERIA: 100%");

    // --- TÍTULO DO JOGO ---
    tft.setTextSize(3);
    tft.setTextColor(~ST77XX_BLACK, ~ST77XX_WHITE); // Texto Branco
    tft.setCursor(45, 40);
    tft.println("P-MOJI CONSOLE");

    // --- DESENHO DOS BOTÕES INTERATIVOS ---
    tft.setTextSize(2);

    // BOTÃO 0: INICIAR
    if (itemSelecionado == 0) {
        // Selecionado: Fundo Vermelho, Texto Preto (Apagado)
        tft.fillRect(50, 100, 220, 40, ~ST77XX_RED);
        tft.setTextColor(~ST77XX_WHITE, ~ST77XX_RED); 
        tft.setCursor(105, 112);
        tft.print("INICIAR");
    } else {
        // Normal: Fundo Azul, Texto Preto (Apagado)
        tft.fillRect(50, 100, 220, 40, ~ST77XX_BLUE);
        tft.setTextColor(~ST77XX_WHITE, ~ST77XX_BLUE);
        tft.setCursor(105, 112);
        tft.print("INICIAR");
    }

    // BOTÃO 1: SCORES
    if (itemSelecionado == 1) {
        // Selecionado: Fundo Vermelho, Texto Preto (Apagado)
        tft.fillRect(50, 155, 220, 40, ~ST77XX_RED);
        tft.setTextColor(~ST77XX_WHITE, ~ST77XX_RED);
        tft.setCursor(115, 167);
        tft.print("SCORES");
    } else {
        // Normal: Fundo Azul, Texto Preto (Apagado)
        tft.fillRect(50, 155, 220, 40, ~ST77XX_BLUE);
        tft.setTextColor(~ST77XX_WHITE, ~ST77XX_BLUE);
        tft.setCursor(115, 167);
        tft.print("SCORES");
    }

    // --- RODAPÉ DE AJUDA ---
    tft.setTextSize(1);
    tft.setTextColor(~ST77XX_YELLOW, ~ST77XX_WHITE); // Texto Amarelo
    tft.setCursor(40, 215);
    tft.print("[VERMELHO] Mudar Botao  |  [VERDE] Confirmar");
}

void desenharTelaScores(int getSimonHighScore, int getReflexHighScore) {
    tft.fillScreen(~ST77XX_WHITE); // Fundo Preto

    tft.setTextSize(3);
    tft.setTextColor(~ST77XX_BLUE, ~ST77XX_WHITE); // Título em Azul Real
    tft.setCursor(65, 30);
    tft.println("TOP SCORES");

    tft.setTextSize(2);
    tft.setTextColor(~ST77XX_BLACK, ~ST77XX_WHITE); // Texto Branco
    tft.setCursor(60, 90);
    tft.print("1. REFLEXO: ");
    tft.print(getReflexHighScore);
    tft.setCursor(60, 130);
    tft.print("2. SIMON: ");
    tft.print(getSimonHighScore);

    tft.setTextSize(1);
    tft.setTextColor(~ST77XX_YELLOW, ~ST77XX_WHITE);
    tft.setCursor(75, 200);
    tft.print("[BRANCO] Voltar ao Menu Principal");
}

void processarNavegacaoMenu() {
  
}