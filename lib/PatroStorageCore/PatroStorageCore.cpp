#include "PatroStorageCore.h"
#include "PatroWifiCore.h"

extern PatroStorageCore storageCore; // Link para acessar o armazenamento

void PatroStorageCore::Init() {
  // Abre o cofre chamado "patroSys". O "false" significa modo Leitura/Escrita
  preferences.begin("patroSys", false);
}

void PatroStorageCore::SaveWifiCredentials(const String &ssid,
                                           const String &password) {
  preferences.putString("wifiSsid", ssid);
  preferences.putString("wifiPass", password);
  Serial.println("Credenciais salvas na memoria Flash com sucesso!");
}

String PatroStorageCore::GetSavedSsid() {
  return preferences.getString("wifiSsid", "");
}

String PatroStorageCore::GetSavedPassword() {
  return preferences.getString("wifiPass", "");
}

bool PatroStorageCore::HasSavedWifi() {
  // Se o tamanho do nome da rede for maior que zero, temos algo salvo
  return GetSavedSsid().length() > 0;
}

void PatroStorageCore::ClearWifiCredentials() {
  preferences.remove("wifiSsid");
  preferences.remove("wifiPass");
  Serial.println("Credenciais apagadas da memoria.");
}