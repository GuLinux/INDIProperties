add_library(indi_properties STATIC switch.cpp number.cpp text.cpp blob.cpp light.cpp)
FILE(GLOB_RECURSE HEADERS "*.h")
add_custom_target(headers SOURCES ${HEADERS})
