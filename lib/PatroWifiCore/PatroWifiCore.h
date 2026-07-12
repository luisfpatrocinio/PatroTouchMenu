#ifndef PATRO_WIFI_CORE_H
#define PATRO_WIFI_CORE_H

#include <Arduino.h>
#include <WiFi.h>

// Máquina de estados para facilitar a vida da nossa UI
enum class WifiState {
  Disconnected,
  Scanning,
  ScanComplete,
  Connecting,
  Connected
};

class PatroWifiCore {
public:
  void Init();
  void Update();
  void StartScan();
  void ConnectToNetwork(const char *ssid, const char *password);

  // Funções de acesso limpas
  WifiState GetState() const { return currentState; }
  int GetFoundNetworksCount() const { return foundNetworks; }
  String GetNetworkName(int index) const;

private:
  WifiState currentState;
  int foundNetworks;
  unsigned long connectionStartTime;

  String targetSsid;
  String targetPassword;
};

#endif