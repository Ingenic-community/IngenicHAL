cmake_minimum_required(VERSION 3.8)
project(IngenicHAL C CXX ASM)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.*" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.*")
add_library(IngenicHAL ${SOURCES})
target_include_directories(IngenicHAL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(ingenic_gpio_x1000 utils/ingenic_gpio.c)
target_compile_definitions(ingenic_gpio_x1000 PRIVATE -DXHAL_CHIP_X1000)
target_link_libraries(ingenic_gpio_x1000 IngenicHAL)

add_executable(ingenic_gpio_x2000 utils/ingenic_gpio.c)
target_compile_definitions(ingenic_gpio_x2000 PRIVATE -DXHAL_CHIP_X2000)
target_link_libraries(ingenic_gpio_x2000 IngenicHAL)

add_executable(ingenic_pwm_x2000 utils/ingenic_pwm.c)
target_compile_definitions(ingenic_pwm_x2000 PRIVATE -DXHAL_CHIP_X2000)
target_link_libraries(ingenic_pwm_x2000 IngenicHAL)

add_executable(st7789_bitbang_x2000 utils/st7789_bitbang.c)
target_compile_definitions(st7789_bitbang_x2000 PRIVATE -DXHAL_CHIP_X2000)
target_link_libraries(st7789_bitbang_x2000 IngenicHAL)
