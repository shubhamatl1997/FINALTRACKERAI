#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    CONTROLLER_UART_0,
    CONTROLLER_UART_1,
    CONTROLLER_UART_2
} controller_uart_port_t;

typedef struct {
    controller_uart_port_t port;
    int tx_pin;
    int rx_pin;
    int baud_rate;
} controller_uart_config_t;

// UART APIs
bool controller_uart_init(const controller_uart_config_t *cfg);
int controller_uart_write(controller_uart_port_t port, const uint8_t *data, int len);
int controller_uart_read(controller_uart_port_t port, uint8_t *data, int max_len, int timeout_ms);

// GPIO APIs
void controller_gpio_set(int gpio, bool level);
void controller_gpio_toggle(int gpio);

// Delay
void controller_delay_ms(uint32_t ms);
