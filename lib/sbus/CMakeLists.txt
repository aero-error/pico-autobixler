add_library(sbus sbus.c)

target_include_directories(sbus PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(sbus pico_stdlib pico_sync hardware_uart hardware_irq)