add_library(crsf crsf.c)

target_include_directories(crsf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(crsf pico_stdlib)