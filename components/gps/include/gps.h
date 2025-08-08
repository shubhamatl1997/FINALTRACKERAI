#pragma once

#include <stdbool.h>

typedef struct {
    int tx_pin;
    int rx_pin;
    int baud_rate;
    int pwr_pin;     // Power control GPIO
    int pps_pin;     // PPS input GPIO (optional)
} gps_config_t;
///////////////////////////////////////////////////////////////GPS_NMEA_PARSER.C//////////////////////////////////////////////////

typedef struct {
    float latitude;
    float longitude;
    bool valid;
} gps_data_t;

// Already in your header or to be added:
bool gps_get_latest(gps_data_t *out);
bool gps_parse_nmea_sentence(const char *sentence);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void gps_init(const gps_config_t *cfg);
void gps_task(void *param);
bool gps_get_location(float *lat, float *lon);  // Updated getter
