#include "HTTPClient.h"
#include "esp_log.h"
#include <cstring>

static const char *TAG = "HTTPClient";

HTTPClient::HTTPClient(const char *url) : url(url), response_data_len(0)
{
    memset(response_data, 0, sizeof(response_data));

    esp_http_client_config_t config = {
        .url = url,
        .event_handler = client_event_handler,
        .user_data = this};
    client = esp_http_client_init(&config);
}

HTTPClient::~HTTPClient()
{
    esp_http_client_cleanup(client);
}

esp_err_t HTTPClient::performRequest(esp_http_client_method_t method, const char *payload)
{
    esp_http_client_set_method(client, method);
    if (payload)
    {
        esp_http_client_set_header(client, "Content-Type", "application/json");
        esp_http_client_set_post_field(client, payload, strlen(payload));
    }
    response_data_len = 0;
    memset(response_data, 0, sizeof(response_data));

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK)
    {
        ESP_LOGI(TAG, "Request succeeded, status=%d, content_length=%d",
                 esp_http_client_get_status_code(client),
                 esp_http_client_get_content_length(client));
    }
    else
    {
        ESP_LOGE(TAG, "Request failed: %s", esp_err_to_name(err));
    }
    return err;
}

const char *HTTPClient::getResponseData() const
{
    return response_data;
}

esp_err_t HTTPClient::client_event_handler(esp_http_client_event_t *evt)
{
    HTTPClient *client_instance = static_cast<HTTPClient *>(evt->user_data);
    if (!client_instance)
    {
        return ESP_ERR_INVALID_ARG;
    }

    switch (evt->event_id)
    {
    case HTTP_EVENT_ERROR:
        ESP_LOGE(TAG, "HTTP_EVENT_ERROR");
        break;
    case HTTP_EVENT_ON_CONNECTED:
        ESP_LOGI(TAG, "HTTP_EVENT_ON_CONNECTED");
        break;
    case HTTP_EVENT_HEADER_SENT:
        ESP_LOGI(TAG, "HTTP_EVENT_HEADER_SENT");
        break;
    case HTTP_EVENT_ON_HEADER:
        ESP_LOGI(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
        break;
    case HTTP_EVENT_ON_DATA:
        if (!esp_http_client_is_chunked_response(client_instance->client))
        {
            int data_len = evt->data_len;
            if (data_len > 0)
            {
                if (client_instance->response_data_len + data_len < RESPONSE_BUFFER_SIZE)
                {
                    memcpy(client_instance->response_data + client_instance->response_data_len, evt->data, data_len);
                    client_instance->response_data_len += data_len;
                    client_instance->response_data[client_instance->response_data_len] = '\0';
                }
                else
                {
                    ESP_LOGW(TAG, "Response data buffer overflow");
                }
            }
        }
        break;
    case HTTP_EVENT_ON_FINISH:
        ESP_LOGI(TAG, "HTTP_EVENT_ON_FINISH");
        break;
    case HTTP_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
        break;
    }
    return ESP_OK;
}
