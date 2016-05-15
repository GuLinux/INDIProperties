add_library(indi_properties STATIC switch.cpp)
FILE(GLOB_RECURSE HEADERS "*.h")
add_custom_target(headers SOURCES ${HEADERS})
