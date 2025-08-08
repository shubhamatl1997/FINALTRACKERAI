#include "gps_startup.h"
#include "controller.h"
#include "esp_log.h"
#include <string.h>
#define TAG "GPS_START"

static controller_uart_port_t gps_uart = CONTROLLER_UART_1;

static const char *start_cmds[] = {
    [GPS_START_HOT]  = "$PMTK101*32\r\n",
    [GPS_START_WARM] = "$PMTK102*31\r\n",
    [GPS_START_COLD] = "$PMTK103*30\r\n"
};

void gps_start_mode(gps_start_mode_t mode)
{
    const char *cmd = start_cmds[mode];
    ESP_LOGI(TAG, "Sending GPS Start Mode: %s", cmd);
    controller_uart_write(gps_uart, (const uint8_t *)cmd, strlen(cmd));
}
