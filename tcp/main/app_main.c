/* MQTT (over TCP) Example
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "esp_event.h"
#include "esp_netif.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "protocol_examples_common.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"

#include "driver/uart.h"

#include "esp_log.h"
#include "mqtt_client.h"

#define BUF_SIZE (1024)
#define TOKENT_SIZE (8)
#define TOPIC "flutter/gate/2"

static const char *TAG = "MQTT_EXAMPLE";

// 开
// FDF403DC41034ADF
#define KAITOKENT "FDF403DC41034ADF"
// 关
// FDF403DC41C04CDF
#define GUANTOKENT "FDF403DC41C04CDF"
// 锁
// FDF403DC410C4CDF
#define SUOTOKENT "FDF403DC410C4CDF"
// 停
// FDF403DC41304CDF
#define TINGTOKENT "FDF403DC41304CDF"


static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event) {

  // 开
  char *Kai = (char *)malloc(TOKENT_SIZE);
  Kai[0] = 0xFD;
  Kai[1] = 0xF4;
  Kai[2] = 0x03;
  Kai[3] = 0xDC;
  Kai[4] = 0x41;
  Kai[5] = 0x03;
  Kai[6] = 0x4A;
  Kai[7] = 0xDF;

  // 关
  char *Guan = (char *)malloc(TOKENT_SIZE);
  Guan[0] = 0xFD;
  Guan[1] = 0xF4;
  Guan[2] = 0x03;
  Guan[3] = 0xDC;
  Guan[4] = 0x41;
  Guan[5] = 0xC0;
  Guan[6] = 0x4C;
  Guan[7] = 0xDF;

  // 锁
  char *Suo = (char *)malloc(TOKENT_SIZE);
  Suo[0] = 0xFD;
  Suo[1] = 0xF4;
  Suo[2] = 0x03;
  Suo[3] = 0xDC;
  Suo[4] = 0x41;
  Suo[5] = 0x0C;
  Suo[6] = 0x4C;
  Suo[7] = 0xDF;

  // 停
  char *Ting = (char *)malloc(TOKENT_SIZE);
  Ting[0] = 0xFD;
  Ting[1] = 0xF4;
  Ting[2] = 0x03;
  Ting[3] = 0xDC;
  Ting[4] = 0x41;
  Ting[5] = 0x30;
  Ting[6] = 0x4C;
  Ting[7] = 0xDF;

  esp_mqtt_client_handle_t client = event->client;
  int msg_id;
  // your_context_t *context = event->context;
  switch (event->event_id) {
  case MQTT_EVENT_CONNECTED:
    // ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
    // msg_id = esp_mqtt_client_publish(client, "/topic/qos1", "data_3", 0, 1,
    // 0); ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);

    msg_id = esp_mqtt_client_subscribe(client, TOPIC, 0);
    ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

    // msg_id = esp_mqtt_client_subscribe(client, "/topic/qos1", 1);
    // ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

    // msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos1");
    // ESP_LOGI(TAG, "sent unsubscribe successful, msg_id=%d", msg_id);
    break;
  case MQTT_EVENT_DISCONNECTED:
    ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
    break;
  case MQTT_EVENT_SUBSCRIBED:
    ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
    msg_id =
        esp_mqtt_client_publish(client, TOPIC, "cheshi", 0, 0, 0);
    ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
    break;
  case MQTT_EVENT_UNSUBSCRIBED:
    ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
    break;
  case MQTT_EVENT_PUBLISHED:
    ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
    break;
  case MQTT_EVENT_DATA:
    ESP_LOGI(TAG, "MQTT_EVENT_DATA");
    char data_type = 'N';

    if (strcmp(event->data, TINGTOKENT) == 0) {
      data_type = 'T';
    } else if (strcmp(event->data, SUOTOKENT) == 0) {
      data_type = 'S';
    } else if (strcmp(event->data, KAITOKENT) == 0) {
      data_type = 'K';
    } else if (strcmp(event->data, GUANTOKENT) == 0) {
      data_type = 'G';
    }

    switch (data_type) {
    case 'G':
      printf("Guan:");
      for (int i = 0; i < TOKENT_SIZE; i++) {
        uart_write_bytes(UART_NUM_1, Guan + i, 1);
        printf("%02x", Guan[i]);
      }
      printf("\n");
      break;
    case 'K':
      printf("Kai:");
      for (int i = 0; i < TOKENT_SIZE; i++) {
        uart_write_bytes(UART_NUM_1, Kai + i, 1);
        printf("%02x", Kai[i]);
      }
      printf("\n");
      break;
    case 'T':
      printf("Ting:");
      for (int i = 0; i < TOKENT_SIZE; i++) {
        uart_write_bytes(UART_NUM_1, Ting + i, 1);
        printf("%02x", Ting[i]);
      }
      printf("\n");
      break;
    case 'S':
      printf("Suo:");
      for (int i = 0; i < TOKENT_SIZE; i++) {
        uart_write_bytes(UART_NUM_1, Suo + i, 1);
        printf("%02x", Suo[i]);
      }
      printf("\n");
      break;
    }

    printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
    printf("DATA=%.*s\r\n", event->data_len, event->data);
    break;
  case MQTT_EVENT_ERROR:
    ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
    break;
  default:
    ESP_LOGI(TAG, "Other event id:%d", event->event_id);
    break;
  }

  free(Guan);
  free(Kai);
  free(Ting);
  free(Suo);
  return ESP_OK;
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base,
                               int32_t event_id, void *event_data) {
  ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base,
           event_id);
  mqtt_event_handler_cb(event_data);
}

static void mqtt_app_start(void) {
  esp_mqtt_client_config_t mqtt_cfg = {
      .username = "Black",
      .password = "1358",
      .uri = CONFIG_BROKER_URL,
      .client_id = "ESP32_BlackHole",
  };
#if CONFIG_BROKER_URL_FROM_STDIN
  char line[128];

  if (strcmp(mqtt_cfg.uri, "FROM_STDIN") == 0) {
    int count = 0;
    printf("Please enter url of mqtt broker\n");
    while (count < 128) {
      int c = fgetc(stdin);
      if (c == '\n') {
        line[count] = '\0';
        break;
      } else if (c > 0 && c < 127) {
        line[count] = c;
        ++count;
      }
      vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    mqtt_cfg.uri = line;
    printf("Broker url: %s\n", line);
  } else {
    ESP_LOGE(TAG, "Configuration mismatch: wrong broker url");
    abort();
  }
#endif /* CONFIG_BROKER_URL_FROM_STDIN */

  esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
  esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler,
                                 client);
  esp_mqtt_client_start(client);
}

static void inituart() {
  uart_config_t uart_config9600 = {.baud_rate = 9600,
                                   .data_bits = UART_DATA_8_BITS,
                                   .parity = UART_PARITY_DISABLE,
                                   .stop_bits = UART_STOP_BITS_1,
                                   .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};
  uart_param_config(UART_NUM_1, &uart_config9600);
  uart_driver_install(UART_NUM_1, BUF_SIZE * 2, 0, 0, NULL, 0);
}

void app_main(void) {
  ESP_LOGI(TAG, "[APP] Startup..");
  ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
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

  /* This helper function configures Wi-Fi or Ethernet, as selected in
   * menuconfig. Read "Establishing Wi-Fi or Ethernet Connection" section in
   * examples/protocols/README.md for more information about this function.
   */

  inituart();

  ESP_ERROR_CHECK(example_connect());

  mqtt_app_start();
}
