#ifndef MESSAGE_HANDLE_H
#define MESSAGE_HANDLE_H

#include "Arduino.h"
#include "ArduinoJson.h"

class messageHandle
{
private:
  /* data */
public:
  messageHandle(/* args */);
  ~messageHandle();
  char* readSerial(int bufferSize);
  bool sendSerial(char* message);
  bool isType(DynamicJsonDocument& json, const char* type);
};

#endif