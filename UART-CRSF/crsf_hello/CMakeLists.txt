if (TARGET tinyusb_device)
    add_executable(crsf_hello
        crsf_hello.c
            )

    # Pull in our pico_stdlib which aggregates commonly used features
    target_link_libraries(crsf_hello pico_stdlib crsf)

    # enable usb output, enable uart output
    pico_enable_stdio_usb(crsf_hello 1)
    pico_enable_stdio_uart(crsf_hello 1)

    # create map/bin/hex/uf2 file etc.
    pico_add_extra_outputs(crsf_hello)

elseif(PICO_ON_DEVICE)
    message(WARNING "not building <crsf_hello> because TinyUSB submodule is not initialized in the SDK")
endif()
