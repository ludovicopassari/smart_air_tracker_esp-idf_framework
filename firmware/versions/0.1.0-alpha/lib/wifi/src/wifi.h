//
// Created by Ludovico Passari on 01/11/24.
//

#ifndef INC_0_1_0_ALPHA_WIFI_H
#define INC_0_1_0_ALPHA_WIFI_H

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_err.h"
#include "esp_system.h"


#define WIFI_SSID "Vodafone-passari"
#define WIFI_PASSWD "filippo300769"


static const char *WIFI_TAG = "WiFiModule";
static const char *TAG_MAIN = "MainModule";

static int s_retry_num = 0;
/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

void init_nvs();

void wifi_init();

void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);


#endif //INC_0_1_0_ALPHA_WIFI_H
