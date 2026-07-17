#include <Arduino.h>
#include <Preferences.h>
#include <TFT_eSPI.h>

#include "PatroLvglManager.h"
#include "PatroStorageCore.h"
#include "PatroTimeCore.h"
#include "PatroUiManager.h"
#include "PatroWifiCore.h"

TFT_eSPI tft = TFT_eSPI();
PatroLvglManager hardwareCore(tft);
PatroStorageCore storageCore;
PatroUiManager interfaceApp;
PatroWifiCore wifiCore;
PatroTimeCore timeCore;

void setup() {
  Serial.begin(115200);

  hardwareCore.Init();
  storageCore.Init();
  interfaceApp.Init();
  wifiCore.Init();
  timeCore.Init();

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
  timeCore.Update();
  delay(5);
}