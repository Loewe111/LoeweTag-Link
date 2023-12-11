#ifndef SERIAL_MANAGER_H
#define SERIAL_MANAGER_H

#include "Arduino.h"
#include "ArduinoJson.h"

class SerialManager
{
private:
  /* data */
public:
  SerialManager(/* args */);
  ~SerialManager();
  char* readSerial(int bufferSize);
  bool sendSerial(char* message);
  bool isType(DynamicJsonDocument& json, const char* type);
};

#endif