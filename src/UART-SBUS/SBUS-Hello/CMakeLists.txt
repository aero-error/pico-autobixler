if (TARGET tinyusb_device)
    add_executable(sbus_hello
        sbus_hello.c
            )

    # Pull in our pico_stdlib which aggregates commonly used features
    target_link_libraries(sbus_hello pico_stdlib sbus)

    # enable usb output, enable uart output
    pico_enable_stdio_usb(sbus_hello 1)
    pico_enable_stdio_uart(sbus_hello 1)

    # create map/bin/hex/uf2 file etc.
    pico_add_extra_outputs(sbus_hello)

elseif(PICO_ON_DEVICE)
    message(WARNING "not building sbus_hello because TinyUSB submodule is not initialized in the SDK")
endif()
