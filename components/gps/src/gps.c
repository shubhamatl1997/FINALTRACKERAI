// #include "controller.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_log.h"
// #include <string.h>

// #define TAG "GPS"

// // Customize for your board
// #define GPS_POWER_GPIO   19
// #define GPS_UART_TX      17
// #define GPS_UART_RX      18

// static controller_uart_port_t gps_uart = CONTROLLER_UART_1;

// void gps_init(void)
// {
//     // 1. Power ON GPS module
//     controller_gpio_set(GPS_POWER_GPIO, true);   // Active HIGH
//     ESP_LOGI(TAG, "GPS powered on");

//     controller_delay_ms(500); // Wait for power to stabilize

//     // 2. Configure UART
//     controller_uart_config_t cfg = {
//         .port = gps_uart,
//         .tx_pin = GPS_UART_TX,
//         .rx_pin = GPS_UART_RX,
//         .baud_rate = 9600
//     };

//     controller_uart_init(&cfg);
//     ESP_LOGI(TAG, "GPS UART initialized");

//     controller_delay_ms(1000); // Give GPS time to boot
// }

// void gps_task(void *param)
// {
//     char c;
//     char nmea_buffer[128];
//     int index = 0;

//     ESP_LOGI(TAG, "GPS task started");

//     while (1)
//     {
//         if (controller_uart_read(gps_uart, (uint8_t *)&c, 1, 100) == 1)
//         {
//             if (c == '\n')
//             {
//                 nmea_buffer[index] = '\0';
//                 ESP_LOGI(TAG, "NMEA: %s", nmea_buffer);
//                 index = 0;
//             }
//             else if (index < sizeof(nmea_buffer) - 1)
//             {
//                 nmea_buffer[index++] = c;
//             }
//         }
//     }
// }

#include "gps.h"
#include "controller.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <string.h>
#include <stdlib.h>

#define TAG "GPS"

static gps_config_t gps_cfg;
static controller_uart_port_t gps_uart = CONTROLLER_UART_1;
static float last_lat = 0, last_lon = 0;

static void parse_nmea(const char *nmea);

void gps_init(const gps_config_t *cfg)
{
    gps_cfg = *cfg;

    // Power on GPS
    controller_gpio_set(gps_cfg.pwr_pin, 1);
    controller_delay_ms(500);  // Let it stabilize

    // Configure UART
    controller_uart_config_t uart_cfg = {
        .port = gps_uart,
        .tx_pin = gps_cfg.tx_pin,
        .rx_pin = gps_cfg.rx_pin,
        .baud_rate = gps_cfg.baud_rate
    };
    controller_uart_init(&uart_cfg);

    ESP_LOGI(TAG, "GPS initialized on UART%d, power pin=%d", gps_uart, gps_cfg.pwr_pin);
}

void gps_task(void *param)
{
    char c;
    char nmea[128];
    int index = 0;

    while (1)
    {
        if (controller_uart_read(gps_uart, (uint8_t *)&c, 1, 100) == 1)
        {
            if (c == '\n')
            {
                nmea[index] = '\0';
                parse_nmea(nmea);
                index = 0;
            }
            else if (index < sizeof(nmea) - 1)
            {
                nmea[index++] = c;
            }
        }
    }
}

bool gps_get_location(float *lat, float *lon)
{
    if (last_lat != 0 && last_lon != 0)
    {
        *lat = last_lat;
        *lon = last_lon;
        return true;
    }
    return false;
}

static void parse_nmea(const char *nmea)
{
    if (strstr(nmea, "$GPGGA") || strstr(nmea, "$GNGGA"))
    {
        char *tokens[15];
        char buffer[128];
        strncpy(buffer, nmea, sizeof(buffer));

        char *token = strtok(buffer, ",");
        int i = 0;
        while (token && i < 15)
        {
            tokens[i++] = token;
            token = strtok(NULL, ",");
        }

        if (i > 5 && strlen(tokens[2]) > 0 && strlen(tokens[4]) > 0)
        {
            float lat = atof(tokens[2]);
            float lon = atof(tokens[4]);

            int lat_deg = (int)(lat / 100);
            float lat_min = lat - lat_deg * 100;
            last_lat = lat_deg + (lat_min / 60.0);

            int lon_deg = (int)(lon / 100);
            float lon_min = lon - lon_deg * 100;
            last_lon = lon_deg + (lon_min / 60.0);

            if (tokens[3][0] == 'S') last_lat = -last_lat;
            if (tokens[5][0] == 'W') last_lon = -last_lon;

            ESP_LOGI(TAG, "Parsed Location: %.6f, %.6f", last_lat, last_lon);
        }
    }
}
