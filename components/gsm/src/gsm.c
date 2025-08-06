#include "gsm.h"
#include "controller.h"
#include "string.h"
#include "esp_log.h"

#define GSM_UART CONTROLLER_UART_1
#define TAG "GSM"

static int pwr_key;
static gsm_event_callback_t event_cb;

static void gsm_power_cycle()
{
    controller_gpio_set(pwr_key, false);
    controller_delay_ms(100);
    controller_gpio_set(pwr_key, true);
    controller_delay_ms(1500);
    controller_gpio_set(pwr_key, false);
    controller_delay_ms(1000);
}

bool gsm_init(int power_key_gpio, gsm_event_callback_t cb)
{
    pwr_key = power_key_gpio;
    event_cb = cb;

    controller_uart_config_t uart_cfg = {
        .port = GSM_UART,
        .tx_pin = 17, // your GSM TX pin
        .rx_pin = 18, // your GSM RX pin
        .baud_rate = 115200
    };
    controller_uart_init(&uart_cfg);
    controller_gpio_set(pwr_key, false);
    gsm_power_cycle();
    return true;
}

bool gsm_send_command(const char *cmd, const char *expect, int timeout_ms)
{
    uint8_t buf[128] = {0};
    controller_uart_write(GSM_UART, (const uint8_t *)cmd, strlen(cmd));
    controller_uart_write(GSM_UART, (const uint8_t *)"\r\n", 2);

    int len = controller_uart_read(GSM_UART, buf, sizeof(buf) - 1, timeout_ms);
    buf[len] = '\0';

    ESP_LOGI(TAG, "CMD: %s --> RESP: %s", cmd, buf);
    return strstr((char *)buf, expect) != NULL;
}

void gsm_task(void *arg)
{
    if (event_cb) event_cb(GSM_STATE_AT_CHECK);
    if (!gsm_send_command("AT", "OK", 1000)) return;

    if (event_cb) event_cb(GSM_STATE_SIM_CHECK);
    if (!gsm_send_command("AT+CPIN?", "READY", 2000)) return;

    if (event_cb) event_cb(GSM_STATE_NETWORK_CHECK);
    if (!gsm_send_command("AT+CREG?", "0,1", 3000)) return;

    if (event_cb) event_cb(GSM_STATE_READY);
}
