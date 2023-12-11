#ifndef MESSAGE_TCP_H
#define MESSAGE_TCP_H

#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>

class messageTCP
{
  private:
    WiFiClient _client;
  public:
    messageTCP();
    ~messageTCP();
    void send(IPAddress ip, char* data);
    void send(IPAddress ip, const uint8_t* data, size_t size);
    uint8_t* receive(WiFiServer* server);
};

#endif