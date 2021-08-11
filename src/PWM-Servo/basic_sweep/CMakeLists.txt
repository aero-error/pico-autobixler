add_executable(basic_sweep
        basic_sweep.c
        )

# Pull in libraries that will be needed
target_link_libraries(basic_sweep pico_stdlib hardware_pwm)

# Create extra executable file formats
pico_add_extra_outputs(basic_sweep)
