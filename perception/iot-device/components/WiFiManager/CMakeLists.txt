idf_component_register(
    SRCS "WiFiManager.cpp"
    INCLUDE_DIRS "include"
    REQUIRES nvs_flash esp_wifi esp_event esp_netif
)
