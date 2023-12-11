#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <ArduinoJson.h>

#include "messageTCP.h"
#include "SerialManager.h"
#include "deviceManager.h"

// User settings
#define AP_SSID "LoeweTag-Link"
#define AP_PASS "12345678"

// Global variables 
#define FIRMWARE "dev 1.1.2"
#define MAX_DEVICES 10
#define SERIAL_BUFFER_SIZE 800
#define CHAR_BUFFER_SIZE 400
#define JSON_BUFFER_SIZE 400
#define IDLE_TIMEOUT 30

// Pin definitions
#define STATUS_LED 2 // Status LED

WiFiServer server(7084);
WiFiClient client;

messageTCP message;
deviceManager devices;
SerialManager serialHandle;

DynamicJsonDocument command(JSON_BUFFER_SIZE);
DynamicJsonDocument response(JSON_BUFFER_SIZE);

void flashLed(int blinkTime) {
  digitalWrite(STATUS_LED, HIGH);
  delay(blinkTime);
  digitalWrite(STATUS_LED, LOW);
}

void sendInfo() {
  response.clear();
  response["type"] = "device_information";
  response["ip"] = WiFi.softAPIP().toString();
  response["device_id"] = 0;
  response["device_type"] = "master";
  response["firmware"] = FIRMWARE;
  serializeJson(response, Serial);
  Serial.println();
}

// Send all the connected devices including itself
void sendDevices() { 
  IPAddress* ips = devices.getDevices();
  int deviceCount = devices.getDeviceCount();
  response.clear();
  response["type"] = "devices";
  JsonArray devices = response.createNestedArray("devices");
  devices.add(WiFi.softAPIP().toString()); // Add itself 
  for(int i=0; i<deviceCount; i++) {
    devices.add(ips[i].toString()); // Add the connected devices
  }
  serializeJson(response, Serial); // Send the connected devices
  Serial.println();
  delete[] ips;
}

void WiFiEvent(WiFiEvent_t event) {
  switch(event) {
    case SYSTEM_EVENT_AP_STAIPASSIGNED:
      sendDevices();
      break;
    case SYSTEM_EVENT_AP_STADISCONNECTED:
      sendDevices();
      break;
    default:
      break;
  }
  flashLed(50);
}

void setup() {
  digitalWrite(STATUS_LED, HIGH);
  delay(2000);
  digitalWrite(STATUS_LED, LOW);
  Serial.begin(115200);
  flashLed(1000);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASS, 1, 0, MAX_DEVICES);
  WiFi.softAPConfig(IPAddress(192, 168, 1, 1), IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));
  WiFi.softAPsetHostname("loewetag-master");
  // set the timeout for the idle connection
  esp_wifi_set_ps(WIFI_PS_NONE);
  WiFi.onEvent(WiFiEvent);

  server.begin();
  flashLed(200);
}

void loop() {
  // WiFi Scan
  IPAddress* ips = devices.getDevices(); // Get the devices
  int deviceCount = devices.getDeviceCount(); // Get the device count

  // TCP Receive
  IPAddress* sender = new IPAddress(); // Create a new IP address
  char* content = message.receive(&server, sender, CHAR_BUFFER_SIZE); // Get the message and the sender

  if(*sender) {
    Serial.println(content);
    flashLed(50);
  }

  // Serial Receive
  if(Serial.available()) { // Serial input
    // char* input = serialHandle.readSerial(CHAR_BUFFER_SIZE);
    DeserializationError err = deserializeJson(command, Serial);

    response.clear(); // Clear the response
    if(err) { // Error
      response["type"] = "error";
      response["message"] = err.c_str();
      // response["content"] = input;
    } else if(serialHandle.isType(command, "get_devices")) { // Get devices
      sendDevices();
    } else if(serialHandle.isType(command, "send")) { // Send message
      String str = command["content"];
      str += '\n';
      const char* data = str.c_str();
      if(command["ip"] == "255.255.255.255") {
        for(int i=0; i<deviceCount; i++) {
          message.send(ips[i], data);
        }
      } else {
        IPAddress ip;
        ip.fromString(command["ip"].as<String>());
        message.send(ip, data);
      }
    } else if(serialHandle.isType(command, "information")) {
      sendInfo();
      for(int i=0; i<deviceCount; i++) {
        message.send(ips[i], "{\"type\":\"information\"}\n"); 
      }
    }
    if(!response.isNull()) {
      serializeJson(response, Serial);
      Serial.println();
    }
    flashLed(50);
  }

  // End of loop
  delete[] ips;
  delete sender;
  delete[] content;
}