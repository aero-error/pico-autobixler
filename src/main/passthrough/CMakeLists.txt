add_executable(flight_rx_passthrough
        flight_rx_passthrough.c
            )

    # Pull in our pico_stdlib which aggregates commonly used features
    target_link_libraries(flight_rx_passthrough pico_stdlib sbus pwm)

    # disable usb output, enable uart output
    pico_enable_stdio_usb(flight_rx_passthrough 0)
    pico_enable_stdio_uart(flight_rx_passthrough 1)

    # create map/bin/hex/uf2 file etc.
    pico_add_extra_outputs(flight_rx_passthrough)