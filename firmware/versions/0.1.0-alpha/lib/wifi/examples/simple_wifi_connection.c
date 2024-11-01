//
// Created by Ludovico Passari on 01/11/24.
//
#include "../src/wifi.h"

void init_nvs() {
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);
}



void app_main() {
  ESP_LOGI(TAG_MAIN, "Program start...");
  init_nvs();
  wifi_init();
}

