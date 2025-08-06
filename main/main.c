#include <stdio.h>             // printf()
#include <stddef.h>            // NULL
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "gsm.h"

void gsm_event_handler(gsm_state_t state)
{
    switch (state) {
        case GSM_STATE_READY:
            printf("GSM READY ✅\n");
            break;
        default:
            printf("GSM STATE: %d\n", state);
            break;
    }
}

void app_main()
{
    gsm_init(23 /* power key GPIO */, gsm_event_handler);
    xTaskCreate(gsm_task, "gsm", 4096, NULL, 5, NULL);
}
