#pragma once

typedef enum {
    GPS_START_HOT,
    GPS_START_WARM,
    GPS_START_COLD
} gps_start_mode_t;

void gps_start_mode(gps_start_mode_t mode);
