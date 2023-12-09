#include "deviceManager.h"

deviceManager::deviceManager(/* args */)
{
}

deviceManager::~deviceManager()
{
}

IPAddress* deviceManager::getDevices() {
  wifi_sta_list_t wifi_sta_list;
  tcpip_adapter_sta_list_t adapter_sta_list;
 
  memset(&wifi_sta_list, 0, sizeof(wifi_sta_list));
  memset(&adapter_sta_list, 0, sizeof(adapter_sta_list));
 
  esp_wifi_ap_get_sta_list(&wifi_sta_list);
  tcpip_adapter_get_sta_list(&wifi_sta_list, &adapter_sta_list);

  IPAddress* ips = new IPAddress[adapter_sta_list.num];

  for (int i = 0; i < adapter_sta_list.num; i++) {
    tcpip_adapter_sta_info_t station = adapter_sta_list.sta[i];
    ips[i] = IPAddress(station.ip.addr);
  }
  return ips;
}

int deviceManager::getDeviceCount() {
  return WiFi.softAPgetStationNum();
}