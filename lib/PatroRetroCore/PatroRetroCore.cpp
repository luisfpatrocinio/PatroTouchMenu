#include "PatroRetroCore.h"

void PatroRetroCore::Init() {
  apiKey = "SUA_CHAVE_AQUI";
  lastFetchTime = 0;
}

void PatroRetroCore::Update() {
  // To be implemented as needed
}

bool PatroRetroCore::FetchPlayerSummary(const String& username) {
  HTTPClient http;
  String url = "https://retroachievements.org/API/API_GetUserSummary.php?u=" + username + "&y=" + apiKey;
  
  http.begin(url);
  int httpResponseCode = http.GET();
  
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    http.end();
    return true;
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
    http.end();
    return false;
  }
}
