cmake_minimum_required(VERSION 3.8)
project(IngenicHAL C CXX ASM)

file(GLOB_RECURSE IngenicHAL_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/common/*.*" "${CMAKE_CURRENT_SOURCE_DIR}/src/peripheral/*.*")
add_library(IngenicHAL ${IngenicHAL_SOURCES})
target_include_directories(IngenicHAL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(ingenic_gpio_x1000 utils/ingenic_gpio.c)
target_compile_definitions(ingenic_gpio_x1000 PRIVATE -DXHAL_CHIP_X1000)
target_link_libraries(ingenic_gpio_x1000 IngenicHAL)

add_executable(ingenic_gpio_x2000 utils/ingenic_gpio.c)
target_compile_definitions(ingenic_gpio_x2000 PRIVATE -DXHAL_CHIP_X2000)
target_link_libraries(ingenic_gpio_x2000 IngenicHAL)

add_executable(x2000_pwm_dma examples/x2000_pwm_dma.c)
target_compile_definitions(x2000_pwm_dma PRIVATE -DXHAL_CHIP_X2000)
target_link_libraries(x2000_pwm_dma IngenicHAL)

add_executable(st7789_bitbang_x2000 examples/st7789_bitbang.c)
target_compile_definitions(st7789_bitbang_x2000 PRIVATE -DXHAL_CHIP_X2000)
target_link_libraries(st7789_bitbang_x2000 IngenicHAL)

add_executable(st7789_bitbang_x1000 examples/st7789_bitbang.c)
target_compile_definitions(st7789_bitbang_x1000 PRIVATE -DXHAL_CHIP_X1000)
target_link_libraries(st7789_bitbang_x1000 IngenicHAL)

add_executable(fb_draw_rect utils/fb_draw_rect.c)

add_executable(minicore_util utils/minicore_util.c)

set(libMiniCore_SOURCES
        src/minicore/init/start.S
        src/minicore/init/exception.S
        src/minicore/init/init.c
        src/minicore/init/irq.c
        )

add_executable(minicore_gpio_flip ${libMiniCore_SOURCES} ${IngenicHAL_SOURCES} examples/minicore_gpio_flip.c)
target_compile_definitions(minicore_gpio_flip PRIVATE -DXHAL_CHIP_X1000)
target_compile_options(minicore_gpio_flip PUBLIC -march=mips32r2 -nostdlib -mno-abicalls -fno-pic -EL -msoft-float -Ofast -membedded-data -fno-math-errno)
target_link_options(minicore_gpio_flip PUBLIC
        -T${CMAKE_CURRENT_SOURCE_DIR}/src/minicore/linker/x1000.lds
        -Wl,--print-memory-usage
        -Wl,--gc-sections
        -march=mips32r2 -nostdlib -mno-abicalls -fno-pic -EL -msoft-float -Ofast -membedded-data -fno-math-errno)
target_include_directories(minicore_gpio_flip PUBLIC src/minicore src/)

add_custom_command(TARGET minicore_gpio_flip POST_BUILD
        COMMAND mipsel-linux-gnu-objcopy --remove-section=.dynsym -O binary minicore_gpio_flip minicore_gpio_flip.bin
        COMMENT "Building BIN file: minicore_gpio_flip.bin")


add_executable(minicore_rgb_lcd ${libMiniCore_SOURCES} ${IngenicHAL_SOURCES} examples/minicore_rgb_lcd.c)
target_compile_definitions(minicore_rgb_lcd PRIVATE -DXHAL_CHIP_X1000)
target_compile_options(minicore_rgb_lcd PUBLIC -march=mips32r2 -nostdlib -mno-abicalls -fno-pic -EL -msoft-float -Ofast -membedded-data -fno-math-errno)
target_link_options(minicore_rgb_lcd PUBLIC
        -T${CMAKE_CURRENT_SOURCE_DIR}/src/minicore/linker/x1000.lds
        -Wl,--print-memory-usage
        -Wl,--gc-sections
        -march=mips32r2 -nostdlib -mno-abicalls -fno-pic -EL -msoft-float -Ofast -membedded-data -fno-math-errno)
target_include_directories(minicore_rgb_lcd PUBLIC src/minicore src/)

add_custom_command(TARGET minicore_rgb_lcd POST_BUILD
        COMMAND mipsel-linux-gnu-objcopy --remove-section=.dynsym -O binary minicore_rgb_lcd minicore_rgb_lcd.bin
        COMMENT "Building BIN file: minicore_rgb_lcd.bin")