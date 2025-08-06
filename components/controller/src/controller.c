#include "controller.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "freertos/task.h"

#define UART_MAP(port) (port == CONTROLLER_UART_1 ? UART_NUM_1 : UART_NUM_2)

bool controller_uart_init(const controller_uart_config_t *cfg)
{
    uart_config_t uart_cfg = {
        .baud_rate = cfg->baud_rate,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_MAP(cfg->port), &uart_cfg);
    uart_set_pin(UART_MAP(cfg->port), cfg->tx_pin, cfg->rx_pin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_MAP(cfg->port), 2048, 0, 0, NULL, 0);
    return true;
}

int controller_uart_write(controller_uart_port_t port, const uint8_t *data, int len)
{
    return uart_write_bytes(UART_MAP(port), (const char *)data, len);
}

int controller_uart_read(controller_uart_port_t port, uint8_t *data, int max_len, int timeout_ms)
{
    return uart_read_bytes(UART_MAP(port), data, max_len, timeout_ms / portTICK_PERIOD_MS);
}

void controller_gpio_set(int gpio, bool level)
{
    gpio_set_level(gpio, level);
}

void controller_gpio_toggle(int gpio)
{
    gpio_set_level(gpio, !gpio_get_level(gpio));
}

void controller_delay_ms(uint32_t ms)
{
    vTaskDelay(ms / portTICK_PERIOD_MS);
}
