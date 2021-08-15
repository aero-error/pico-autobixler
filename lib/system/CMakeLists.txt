add_library(system system.c)

target_include_directories(system PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(system pico_stdlib pico_sync hardware_uart hardware_irq)