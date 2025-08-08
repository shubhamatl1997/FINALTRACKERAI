# idf_component_register(
#     SRCS "src/gps.c"
#     INCLUDE_DIRS "include"
#     REQUIRES controller)
                       
idf_component_register(
    SRCS    "src/gps.c"
            "src/gps_nmea_parser.c"
            "src/gps_startup.c"
    INCLUDE_DIRS "include"
    REQUIRES controller driver freertos log
)
