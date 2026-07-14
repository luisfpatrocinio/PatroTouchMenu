#include "PatroTimeCore.h"
#include <time.h>

void PatroTimeCore::Init() {
  configTime(-10800, 0, "pool.ntp.org", "time.nist.gov");
}

void PatroTimeCore::Update() {
  // Opcional, para futuras implementações caso necessário
}

String PatroTimeCore::GetFormattedTime() {
  struct tm timeInfo;
  if (!getLocalTime(&timeInfo, 10)) {
    return "--:--";
  }
  
  char timeBuffer[10];
  sprintf(timeBuffer, "%02d:%02d", timeInfo.tm_hour, timeInfo.tm_min);
  return String(timeBuffer);
}
