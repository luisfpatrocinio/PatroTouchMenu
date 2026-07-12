#include "PatroMenu.h"
#include <Arduino.h>
#include <TFT_eSPI.h>

// Instancia o objeto global da tela
TFT_eSPI tft = TFT_eSPI();

// Cria o gerenciador do menu passando a instância da tela
PatroMenu menu(tft);

void setup() {
  Serial.begin(115200);

  // Inicializa o menu modular
  menu.Init();

  Serial.println("Menu inicializado com sucesso!");
}

void loop() {
  // Monitora constantemente se houve toques na tela
  menu.CheckTouch();

  // Mantém a animação rodando
  menu.UpdateAnimation();

  // O ESP32 fica livre para processar outras tarefas leves aqui
  delay(10);
}