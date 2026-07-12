#include "PatroWifiCore.h"
#include "PatroStorageCore.h"

extern PatroStorageCore storageCore;

void PatroWifiCore::Init() {
  WiFi.mode(WIFI_STA); // Modo Estação (Cliente)
  WiFi.disconnect();
  currentState = WifiState::Disconnected;
  foundNetworks = 0;
}

void PatroWifiCore::Update() {
  if (currentState == WifiState::Scanning) {
    int scanResult = WiFi.scanComplete();

    if (scanResult >= 0) {
      foundNetworks = scanResult;
      currentState = WifiState::ScanComplete;
      Serial.print("Scan concluido! Redes encontradas: ");
      Serial.println(foundNetworks);
    }
    // -1 significa WIFI_SCAN_RUNNING. Está tudo normal, o rádio está
    // trabalhando.
    else if (scanResult == -1) {
      // Apenas aguarda pacificamente
    }
    // -2 significa WIFI_SCAN_FAILED. Aqui entra a nossa proteção K.I.S.S.
    else if (scanResult == -2) {
      // Só consideramos que falhou de verdade se já se passou 1 segundo do
      // pedido
      if (millis() - connectionStartTime > 1000) {
        currentState = WifiState::Disconnected;
        Serial.println("Falha real no scan de redes.");
      }
    }
  } else if (currentState == WifiState::Connecting) {
    if (WiFi.status() == WL_CONNECTED) {
      currentState = WifiState::Connected;
      Serial.println("Wi-Fi Conectado com sucesso!");

      // Salva na memória permanentemente!
      storageCore.SaveWifiCredentials(targetSsid, targetPassword);
    }
    // Timeout de 10 segundos para não ficar preso
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

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(200); // Dá um tempinho extra pro rádio respirar

    Serial.println("Iniciando scan SINCRONO (Teste de Hardware)...");

    // Passar 'false' trava o processador até o scan terminar!
    int n = WiFi.scanNetworks(false);

    if (n >= 0) {
      foundNetworks = n;
      currentState = WifiState::ScanComplete;
      Serial.print("Sucesso! Redes encontradas: ");
      Serial.println(foundNetworks);
    } else {
      currentState = WifiState::Disconnected;
      Serial.print("Falha critica de hardware. Codigo de erro: ");
      Serial.println(n);
    }
  }
}

void PatroWifiCore::ConnectToNetwork(const char *ssid, const char *password) {
  if (currentState != WifiState::Connecting) {
    currentState = WifiState::Connecting;
    connectionStartTime = millis();

    // Salva os dados localmente para uso futuro
    targetSsid = String(ssid);
    targetPassword = String(password);

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