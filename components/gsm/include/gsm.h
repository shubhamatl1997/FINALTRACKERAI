#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    GSM_STATE_IDLE,
    GSM_STATE_AT_CHECK,
    GSM_STATE_SIM_CHECK,
    GSM_STATE_NETWORK_CHECK,
    GSM_STATE_READY
} gsm_state_t;

typedef void (*gsm_event_callback_t)(gsm_state_t state);

bool gsm_init(int power_key_gpio, gsm_event_callback_t cb);
void gsm_task(void *arg);
bool gsm_send_command(const char *cmd, const char *expect, int timeout_ms);
