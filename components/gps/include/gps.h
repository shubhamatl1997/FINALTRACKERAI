// #pragma once

// #include <stdbool.h>
// #include <stdint.h>

// typedef struct {
//     bool valid_fix;
//     float latitude;
//     float longitude;
//     float speed_kmph;
//     int num_satellites;
//     char timestamp[11]; // hhmmss.sss
//     char date[7];       // ddmmyy
// } gps_location_t;

// typedef enum {
//     GPS_STATE_NOT_READY,
//     GPS_STATE_READY,
//     GPS_STATE_ERROR
// } gps_state_t;

// typedef void (*gps_event_cb_t)(gps_state_t state, const gps_location_t *loc);

// void gps_init(int uart_num, gps_event_cb_t cb);
// void gps_task(void *param);


#pragma once

#include <stdbool.h>

typedef struct {
    int tx_pin;
    int rx_pin;
    int baud_rate;
    int pwr_pin;     // Power control GPIO
    int pps_pin;     // PPS input GPIO (optional)
} gps_config_t;

void gps_init(const gps_config_t *cfg);
void gps_task(void *param);
bool gps_get_location(float *lat, float *lon);  // Updated getter
