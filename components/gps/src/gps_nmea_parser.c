// gps_nmea_parser.c
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "gps.h"
#include "esp_log.h"


#include "gps.h"

#define TAG "GPS_NMEA"

static gps_data_t latest_fix = { 0 };

// Utility to convert NMEA lat/lng format to decimal degrees
static float nmea_to_decimal(const char *nmea_val, const char *direction)
{
    if (!nmea_val || !direction || strlen(nmea_val) < 4) return 0.0f;

    float degrees = 0;
    float minutes = 0;

    int deg_width = (strchr(nmea_val, '.') - nmea_val > 4) ? 3 : 2;
    char deg_str[4] = {0};
    char min_str[16] = {0};

    strncpy(deg_str, nmea_val, deg_width);
    strcpy(min_str, nmea_val + deg_width);

    degrees = atof(deg_str);
    minutes = atof(min_str);

    float result = degrees + (minutes / 60.0f);

    if (direction[0] == 'S' || direction[0] == 'W') result *= -1;

    return result;
}

bool gps_parse_nmea_sentence(const char *nmea)
{
    if (!nmea || strlen(nmea) < 6) return false;

    if (strstr(nmea, "$GNRMC") || strstr(nmea, "$GPRMC")) {
        char *token;
        char buf[128];
        strncpy(buf, nmea, sizeof(buf));

        token = strtok(buf, ","); // $GNRMC

        token = strtok(NULL, ","); // Time
        token = strtok(NULL, ","); // Status (A = valid)
        if (token == NULL || token[0] != 'A') {
            ESP_LOGW(TAG, "RMC: No fix");
            latest_fix.valid = false;
            return false;
        }

        token = strtok(NULL, ","); // Latitude
        if (!token) return false;
        char lat[16];
        strncpy(lat, token, sizeof(lat));

        token = strtok(NULL, ","); // N/S
        if (!token) return false;
        char lat_dir = token[0];

        token = strtok(NULL, ","); // Longitude
        if (!token) return false;
        char lon[16];
        strncpy(lon, token, sizeof(lon));

        token = strtok(NULL, ","); // E/W
        if (!token) return false;
        char lon_dir = token[0];

        latest_fix.latitude = nmea_to_decimal(lat, &lat_dir);
        latest_fix.longitude = nmea_to_decimal(lon, &lon_dir);
        latest_fix.valid = true;

        ESP_LOGI(TAG, "Parsed Fix: Lat=%.6f, Lon=%.6f", latest_fix.latitude, latest_fix.longitude);
        return true;
    }

    return false;
}

bool gps_get_latest(gps_data_t *out)
{
    if (!out) return false;
    *out = latest_fix;
    return latest_fix.valid;
}
