//
// Created by Ludovico Passari on 01/11/24.
//
#include "wifi.h"
#include "nvs_utils.h"


void app_main() {
  ESP_LOGI("MAIN", "Program start...");
  init_nvs();
  wifi_init();
}

