#include <Arduino.h>
#include <TFT_eSPI.h>
#include "PatroLvglManager.h"

TFT_eSPI tft = TFT_eSPI();
PatroLvglManager uiManager(tft);

void setup() {
    Serial.begin(115200);
    
    uiManager.Init();
    uiManager.CreateTestUi();
}

void loop() {
    uiManager.Update();
    delay(5); // ESP32 respira por 5ms (sincronizado com o lv_tick_inc)
}