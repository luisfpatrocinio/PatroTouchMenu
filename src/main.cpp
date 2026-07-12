#include "PatroLvglManager.h"
#include "PatroUiManager.h"
#include "PatroWifiCore.h" // Importa o nosso novo modulo
#include <Arduino.h>
#include <TFT_eSPI.h>


TFT_eSPI tft = TFT_eSPI();
PatroLvglManager hardwareCore(tft);
PatroUiManager interfaceApp;

// Instancia globalmente para a UI poder acessá-lo depois
PatroWifiCore wifiCore;

void setup() {
  Serial.begin(115200);

  hardwareCore.Init();
  interfaceApp.Init();
  wifiCore.Init(); // Inicia o rádio do ESP32
}

void loop() {
  hardwareCore.Update();
  interfaceApp.Update();
  wifiCore.Update(); // Mantém o estado da rede atualizado
  delay(5);
}