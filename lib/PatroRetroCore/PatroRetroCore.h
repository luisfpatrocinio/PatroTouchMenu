#ifndef PATRO_RETRO_CORE_H
#define PATRO_RETRO_CORE_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

class PatroRetroCore {
public:
  void Init();
  void Update();
  bool FetchPlayerSummary(const String& username);

private:
  String apiKey;
  unsigned long lastFetchTime;
};

#endif
