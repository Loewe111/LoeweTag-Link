#include "messageHandle.h"

messageHandle::messageHandle(/* args */)
{
}

messageHandle::~messageHandle()
{
}

char* messageHandle::readSerial(int bufferSize) {
  char* buffer = new char[bufferSize];
  int index = 0;
  while (index < bufferSize - 1) {
    if (Serial.available()) {
      char c = Serial.read();
      if (c == '\n') {
        index++;
        break;
      } else if (c != '\r') {
        buffer[index] = c;
        index++;
      }
    }
  }
  buffer[index] = '\0';  // incomplete line, terminate the string
  return buffer;
}

bool messageHandle::sendSerial(char* message) {
  Serial.println(message);
  return true;
}

bool messageHandle::isType(DynamicJsonDocument& json, const char* type) {
  return json.containsKey("type") && json["type"] == type;
}