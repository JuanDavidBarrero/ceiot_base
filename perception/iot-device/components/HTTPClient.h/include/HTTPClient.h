#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include "esp_err.h"
#include "esp_http_client.h"

#define RESPONSE_BUFFER_SIZE 2048  // Define a fixed buffer size

class HTTPClient {
public:
    HTTPClient(const char* url);
    ~HTTPClient();

    esp_err_t performRequest(esp_http_client_method_t method, const char* payload = nullptr);
    const char* getResponseData() const;

private:
    static esp_err_t client_event_handler(esp_http_client_event_t *evt);
    esp_http_client_handle_t client;
    const char* url;
    char response_data[RESPONSE_BUFFER_SIZE];
    int response_data_len;
};

#endif // HTTP_CLIENT_H
