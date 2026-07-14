#ifndef PATRO_TIME_CORE_H
#define PATRO_TIME_CORE_H

#include <Arduino.h>

class PatroTimeCore {
public:
  void Init();
  void Update();
  String GetFormattedTime();
};

#endif
