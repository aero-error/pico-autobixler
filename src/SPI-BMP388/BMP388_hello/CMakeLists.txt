add_executable(BMP388_hello
        BMP388_hello.c
        )

# Pull in our pico_stdlib which aggregates commonly used features
target_link_libraries(BMP388_hello pico_stdlib hardware_spi)

# enable usb output
pico_enable_stdio_usb(BMP388_hello 1)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(BMP388_hello)