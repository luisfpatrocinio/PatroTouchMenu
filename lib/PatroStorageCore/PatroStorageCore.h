#ifndef PATRO_STORAGE_CORE_H
#define PATRO_STORAGE_CORE_H

#include <Arduino.h>
#include <Preferences.h>

class PatroStorageCore {
public:
  void Init();
  void SaveWifiCredentials(const String &ssid, const String &password);
  String GetSavedSsid();
  String GetSavedPassword();
  bool HasSavedWifi();
  void ClearWifiCredentials();

  void SaveThemeColor(uint32_t hexColor);
  uint32_t GetThemeColor();

  void SaveRetroUsername(const String &username);
  String GetRetroUsername();
  bool HasRetroUsername();

private:
  Preferences preferences;
};

#endif