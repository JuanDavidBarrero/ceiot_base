#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include <string.h>

class WiFiManager {
public:
    WiFiManager();
    void connect(const char* ssid, const char* password);
    void disconnect();
    bool isConnected();

private:
    static void eventHandler(void* arg, esp_event_base_t event_base,
                             int32_t event_id, void* event_data);
    static bool isIpAvailable ;
};

#endif // WIFI_MANAGER_H
