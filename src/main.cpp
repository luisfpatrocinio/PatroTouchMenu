#include "PatroLvglManager.h"
#include "PatroUiManager.h" // Novo gerente de interface
#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();
PatroLvglManager hardwareCore(tft);
PatroUiManager interfaceApp;

void setup() {
  Serial.begin(115200);

  hardwareCore.Init(); // Liga a tela e o touch no LVGL
  interfaceApp.Init(); // Desenha o Layout Mestre e o Menu!
}

void loop() {
  hardwareCore.Update();
  delay(5);
}