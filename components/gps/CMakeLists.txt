# idf_component_register(
#     SRCS "src/gps.c"
#     INCLUDE_DIRS "include"
#     REQUIRES controller)
                       
idf_component_register(
    SRCS "src/gps.c"
    INCLUDE_DIRS "include"
    REQUIRES controller driver freertos log
)
