//
// Created by Ludovico Passari on 01/11/24.
//

#ifndef INC_0_1_0_ALPHA_WIFI_UTILS_H
#define INC_0_1_0_ALPHA_WIFI_UTILS_H

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_system.h"

// for test only
#define WIFI_SSID ""
#define WIFI_PASSWD ""

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1
#define MAX_RETRY 5  // Maximum number of reconnection attempts

void wifi_init(void);

void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

#endif //INC_0_1_0_ALPHA_WIFI_UTILS_H
