#include "PatroWifiCore.h"

void PatroWifiCore::Init() {
  WiFi.mode(WIFI_STA); // Modo Estação (Cliente)
  WiFi.disconnect();
  currentState = WifiState::Disconnected;
  foundNetworks = 0;
}

void PatroWifiCore::Update() {
  // Avalia o que fazer baseado no estado atual
  if (currentState == WifiState::Scanning) {
    // scanComplete retorna valores negativos se ainda estiver procurando
    int scanResult = WiFi.scanComplete();

    if (scanResult >= 0) {
      foundNetworks = scanResult;
      currentState = WifiState::ScanComplete;
      Serial.print("Scan concluido! Redes encontradas: ");
      Serial.println(foundNetworks);
    } else if (scanResult == WIFI_SCAN_FAILED) {
      currentState = WifiState::Disconnected;
      Serial.println("Falha no scan de redes.");
    }
  } else if (currentState == WifiState::Connecting) {
    if (WiFi.status() == WL_CONNECTED) {
      currentState = WifiState::Connected;
      Serial.println("Wi-Fi Conectado com sucesso!");
    }
    // Timeout de 10 segundos para não ficar preso tentando conectar
    else if (millis() - connectionStartTime > 10000) {
      WiFi.disconnect();
      currentState = WifiState::Disconnected;
      Serial.println("Falha ao conectar (Timeout).");
    }
  }
}

void PatroWifiCore::StartScan() {
  if (currentState != WifiState::Scanning &&
      currentState != WifiState::Connecting) {
    currentState = WifiState::Scanning;

    // O parametro 'true' ativa o modo assíncrono!
    WiFi.scanNetworks(true);
    Serial.println("Iniciando scan assincrono...");
  }
}

void PatroWifiCore::ConnectToNetwork(const char *ssid, const char *password) {
  if (currentState != WifiState::Connecting) {
    currentState = WifiState::Connecting;
    connectionStartTime = millis();
    WiFi.begin(ssid, password);

    Serial.print("Tentando conectar a: ");
    Serial.println(ssid);
  }
}

String PatroWifiCore::GetNetworkName(int index) const {
  if (index >= 0 && index < foundNetworks) {
    return WiFi.SSID(index);
  }
  return "";
}