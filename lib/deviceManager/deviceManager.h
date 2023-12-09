#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H

#include "Arduino.h"
#include "esp_wifi.h"
#include "WiFi.h"

class deviceManager
{
private:
  /* data */
public:
  deviceManager(/* args */);
  ~deviceManager();
  IPAddress* getDevices();
  int getDeviceCount();
};

#endif