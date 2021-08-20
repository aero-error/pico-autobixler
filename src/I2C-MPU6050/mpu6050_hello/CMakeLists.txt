add_executable(mpu6050_hello
        mpu6050_hello.c
        )

# Pull in our (to be renamed) simple get you started dependencies
target_link_libraries(mpu6050_hello pico_stdlib hardware_i2c mpu6050)

# create map/bin/hex file etc.
pico_add_extra_outputs(mpu6050_hello)

