#include "taskDisplay.h"

// Instanciação única do objeto global do display
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// Função para ser chamada uma única vez na inicialização geral do console
void inicializarDisplay() {
  pinMode(TFT_LED, OUTPUT);
  analogWrite(TFT_LED, 128); // Proteção do backlight

  pinMode(TFT_RST, OUTPUT);
  digitalWrite(TFT_RST, LOW);
  delay(100);
  digitalWrite(TFT_RST, HIGH);
  delay(100);

  tft.init(240, 320); 
  tft.invertDisplay(true); 
  tft.setRotation(1); 
  tft.fillScreen(~ST77XX_WHITE); // Fundo preto real
}

// Esta é a função que vai controlar a escrita, sem usar o nome "loop"
void desenharTelaStatus() {
  tft.setTextSize(3); 
  tft.setTextColor(~ST77XX_RED, ~ST77XX_WHITE); 
  tft.setCursor(50, 40); 
  tft.println("P-MOJI CONSOLE");

  tft.setTextSize(2); 
  tft.setTextColor(~ST77XX_BLACK, ~ST77XX_WHITE); 
  tft.setCursor(60, 100); 
  tft.print("Status: ");
  
  tft.setTextColor(~ST77XX_GREEN, ~ST77XX_WHITE); 
  tft.println("ONLINE");

  tft.setTextSize(2);
  tft.setTextColor(~ST77XX_BLACK, ~ST77XX_WHITE);
  tft.setCursor(60, 150);
  tft.print("Bateria: 100%");
}