#include <stdio.h>             // printf()
#include <stddef.h>            // NULL
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "gsm.h"
#include "gps.h"

// void gsm_event_handler(gsm_state_t state)
// {
//     switch (state) {
//         case GSM_STATE_READY:
//             printf("GSM READY ✅\n");
//             break;
//         default:
//             printf("GSM STATE: %d\n", state);
//             break;
//     }
// }

// void app_main()
// {
//     gsm_init(23 /* power key GPIO */, gsm_event_handler);
//     xTaskCreate(gsm_task, "gsm", 4096, NULL, 5, NULL);
// }


// void gps_event_handler(gps_state_t state, const gps_location_t *loc)
// {
//     if (state == GPS_STATE_READY && loc->valid_fix) {
//         printf("GPS: %.6f, %.6f, Speed: %.2f km/h, Sats: %d\n",
//             loc->latitude, loc->longitude, loc->speed_kmph, loc->num_satellites);
//     }
// }

void app_main(void)
{
    gps_config_t gps = {
        .tx_pin = 17,
        .rx_pin = 18,
        .baud_rate = 9600,
        .pwr_pin = 5,
        .pps_pin = -1  // optional
    };
    gps_init(&gps);
    xTaskCreate(gps_task, "gps", 4096, NULL, 5, NULL);
}

