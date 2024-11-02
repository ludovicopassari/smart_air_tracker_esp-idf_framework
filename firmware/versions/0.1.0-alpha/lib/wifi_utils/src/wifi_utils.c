//
// Created by Ludovico Passari on 01/11/24.
//

#include "wifi_utils.h"

static const char *WIFI_TAG = "WiFiModule";

// FreeRTOS event group to signal when we are connected
static EventGroupHandle_t s_wifi_event_group;
static int s_retry_num = 0;

void wifi_init(void) {
  // Create an event group to handle Wi-Fi connection status
  s_wifi_event_group = xEventGroupCreate();

  // Initialize the network interface
  ESP_ERROR_CHECK(esp_netif_init()); // This line initializes the ESP32 network interface
  ESP_ERROR_CHECK(esp_event_loop_create_default()); //esp_event_loop_create_default() initializes the event loop that handles events like Wi-Fi connection status changes
  esp_netif_create_default_wifi_sta(); //This line creates a default Wi-Fi station interface. It configures a network interface that can be used to connect to a Wi-Fi access point in station mode

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));

  // Register event handlers for Wi-Fi and IP events
  esp_event_handler_instance_t instance_any_id;
  esp_event_handler_instance_t instance_got_ip;

  ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                      ESP_EVENT_ANY_ID,
                                                      &wifi_event_handler,
                                                      NULL,
                                                      &instance_any_id));

  ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                      IP_EVENT_STA_GOT_IP,
                                                      &wifi_event_handler,
                                                      NULL,
                                                      &instance_got_ip));
  wifi_config_t wifi_config = {
          .sta = {
                  .ssid = WIFI_SSID,
                  .password = WIFI_PASSWD
          },
  };
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
  ESP_ERROR_CHECK(esp_wifi_start()); // Start Wi-Fi

  ESP_LOGI(WIFI_TAG, "Wi-Fi initialization completed. Connecting...");

  // Wait until either connection is established or connection attempt fails
  EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                                         WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                         pdFALSE,
                                         pdFALSE,
                                         portMAX_DELAY);

  /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
    * happened. */
  if (bits & WIFI_CONNECTED_BIT) {
    ESP_LOGI(WIFI_TAG, "connected to ap SSID:%s password:%s", WIFI_SSID, WIFI_PASSWD);
  } else if (bits & WIFI_FAIL_BIT) {
    ESP_LOGI(WIFI_TAG, "Failed to connect to SSID:%s, password:%s", WIFI_SSID, WIFI_PASSWD);
  } else {
    ESP_LOGE(WIFI_TAG, "UNEXPECTED EVENT");
  }
}

// Wi-Fi event handler
void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
  if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
    // Start connection when Wi-Fi starts
    esp_wifi_connect();
  } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
    if (s_retry_num < MAX_RETRY) {
      // Retry connection if the maximum number of retries is not reached
      esp_wifi_connect();
      s_retry_num++;
      ESP_LOGI(WIFI_TAG, "Retrying connection to the access point...");
    } else {
      // Set the failure bit if retries are exhausted
      xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
    }
    ESP_LOGI(WIFI_TAG, "Failed to connect to the access point");
  } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
    ip_event_got_ip_t *event = (ip_event_got_ip_t *) event_data;
    ESP_LOGI(WIFI_TAG, "Successfully connected with IP address: " IPSTR, IP2STR(&event->ip_info.ip));
    s_retry_num = 0;
    xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
  }
}
