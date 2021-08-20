add_library(mpu6050 mpu6050.c)

target_include_directories(mpu6050 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(mpu6050 pico_stdlib hardware_i2c)