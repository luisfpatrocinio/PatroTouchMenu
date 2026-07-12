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

private:
  Preferences preferences;
};

#endif