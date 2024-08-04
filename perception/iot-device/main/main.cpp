#include <iostream>
#include <cstdio>
#include "esp_log.h"
#include "esp_system.h"
#include "esp_mac.h"
#include "bmp280.h"
#include "WiFiManager.h"
#include "HTTPClient.h"
#include "sdkconfig.h" // Include this to access the configuration options

#define SDA_GPIO GPIO_NUM_21
#define SCL_GPIO GPIO_NUM_22

static bmp280_t temp_sensor;

extern "C" void app_main(void);

char json_string[256];

void app_main()
{
    const char *TAG = "Main";
    ESP_LOGI(TAG, "ESP32 JSON Example");

    WiFiManager wifiManager;

    const char *ssid = CONFIG_WIFI_SSID;
    const char *password = CONFIG_WIFI_PASSWORD;
    const char *key = CONFIG_DEVICE_KEY;
    const char *devName = CONFIG_DEVICE_NAME;

    wifiManager.connect(ssid, password);

    while (!wifiManager.isConnected())
    {
        ESP_LOGI(TAG, "Waiting for connection...");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    ESP_LOGI(TAG, "Device connected to WiFi");

    i2cdev_init();

    memset(&temp_sensor, 0, sizeof(bmp280_t));
    temp_sensor.i2c_dev.timeout_ticks = 0xffff / portTICK_PERIOD_MS;

    bmp280_params_t params;
    bmp280_init_default_params(&params);

    bmp280_init_desc(&temp_sensor, BMP280_I2C_ADDRESS_1, 0, SDA_GPIO, SCL_GPIO);
    bmp280_init(&temp_sensor, &params);
    float pressure, temperature, humidity;

    unsigned char mac_base[6] = {0};
    esp_efuse_mac_get_default(mac_base);

    char mac_str[18];
    snprintf(mac_str, sizeof(mac_str), "%02X:%02X:%02X:%02X:%02X:%02X",
             mac_base[0], mac_base[1], mac_base[2], mac_base[3], mac_base[4], mac_base[5]);

    sprintf(json_string, "{\"id\":\"%s\",\"n\":\"%s\",\"k\":\"%s\"}", mac_str, devName, key);

    HTTPClient httpClient("http://192.168.1.46:8080/device");
    esp_err_t err = httpClient.performRequest(HTTP_METHOD_POST, json_string);
    if (err == ESP_OK)
    {
        const char *response = httpClient.getResponseData();
        if (response)
        {
            ESP_LOGI(TAG, "\r\nResponse Data: %s\r\n", response);
        }
    }
    else
    {
        ESP_LOGE(TAG, "HTTP POST to /devices/ failed: %s", esp_err_to_name(err));
    }

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(10000));
        if (bmp280_read_float(&temp_sensor, &temperature, &pressure, &humidity) == ESP_OK)
        {
            // Correctly format the JSON string for sensor data
            snprintf(json_string, sizeof(json_string),
                     "{\"id\":\"%s\",\"key\":\"%s\",\"t\":%.1f,\"h\":%.1f,\"p\":%.1f}",
                     mac_str, key, temperature, humidity, pressure);

            ESP_LOGI(TAG, "JSON Data: %s", json_string);
            HTTPClient httpClient("http://192.168.1.46:8080/measurement");
            esp_err_t err = httpClient.performRequest(HTTP_METHOD_POST, json_string);
            if (err == ESP_OK)
            {
                const char *response = httpClient.getResponseData();
                if (response)
                {
                    ESP_LOGI(TAG, "\r\nResponse Data: %s\r\n", response);
                }
            }
            else
            {
                ESP_LOGE(TAG, "HTTP POST to /measurement failed: %s", esp_err_to_name(err));
            }
        }
        else
        {
            ESP_LOGE(TAG, "Failed to read from BMP280 sensor");
        }
    }
}
