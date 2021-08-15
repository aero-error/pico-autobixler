add_library(pwm pwm.c)

target_include_directories(pwm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(pwm pico_stdlib pico_time pico_sync hardware_pwm hardware_clocks system)