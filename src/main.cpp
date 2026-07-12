#include "PatroLvglManager.h"
#include "PatroStorageCore.h"
#include "PatroUiManager.h"
#include "PatroWifiCore.h"
#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();
PatroLvglManager hardwareCore(tft);
PatroStorageCore storageCore;
PatroUiManager interfaceApp;
PatroWifiCore wifiCore;

void setup() {
  Serial.begin(115200);

  hardwareCore.Init();
  interfaceApp.Init();
  storageCore.Init();
  wifiCore.Init();

  // Sistema de Auto-Connect
  if (storageCore.HasSavedWifi()) {
    Serial.println("Rede salva encontrada! Conectando automaticamente...");
    wifiCore.ConnectToNetwork(storageCore.GetSavedSsid().c_str(),
                              storageCore.GetSavedPassword().c_str());
  }
}

void loop() {
  hardwareCore.Update();
  interfaceApp.Update();
  wifiCore.Update();
  delay(5);
}