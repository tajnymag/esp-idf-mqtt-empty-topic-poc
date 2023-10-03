/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */
/* C++ MQTT (over TCP) Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_mqtt.hpp"
#include "esp_mqtt_client_config.hpp"

#include "wifi.h"

namespace mqtt = idf::mqtt;

namespace {
constexpr auto *TAG = "MQTT_EXAMPLE";

class MyClient final : public mqtt::Client {
public:
    using mqtt::Client::Client;

private:
    void on_connected(esp_mqtt_event_handle_t const event) override
    {
        subscribe(example_topic.get());
    }
    void on_data(esp_mqtt_event_handle_t const event) override
    {
        ESP_LOGI(TAG, "Received message with msg_id: %d", event->msg_id);
        ESP_LOGI(TAG, " - event_id: %d", event->event_id);
        ESP_LOGI(TAG, " - topic: %.*s", event->topic_len, event->topic);
        ESP_LOGI(TAG, " - topic_len: %d", event->topic_len);
        ESP_LOGI(TAG, " - total_data_len: %d", event->total_data_len);
        ESP_LOGI(TAG, " - data_len: %d", event->data_len);
        ESP_LOGI(TAG, " - current_data_offset: %d", event->current_data_offset);

        if (example_topic.match(event->topic, event->topic_len)) {
            ESP_LOGI(TAG, "Received in the messages topic");
        }
    }
    mqtt::Filter example_topic{"example/topic"};
};
}

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %" PRIu32 " bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("MQTT_CLIENT", ESP_LOG_VERBOSE);
    esp_log_level_set("MQTT_EXAMPLE", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_TCP", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_SSL", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
    esp_log_level_set("OUTBOX", ESP_LOG_VERBOSE);

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Helper function to connect to the internet (addapted from wifi example)
    ESP_ERROR_CHECK(wifi_init_sta());

    mqtt::BrokerConfiguration broker{
        .address = {mqtt::URI{std::string{CONFIG_BROKER_URL}}},
        .security =  mqtt::Insecure{}
    };
    mqtt::ClientCredentials credentials{};
    mqtt::Configuration config{};

    MyClient client{broker, credentials, config};

    while (true) {
        constexpr TickType_t xDelay = 500 / portTICK_PERIOD_MS;
        vTaskDelay(xDelay);
    }
}