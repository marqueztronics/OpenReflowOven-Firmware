################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwjson/src/lwjson/lwjson.c \
../lwjson/src/lwjson/lwjson_debug.c \
../lwjson/src/lwjson/lwjson_stream.c 

OBJS += \
./lwjson/src/lwjson/lwjson.o \
./lwjson/src/lwjson/lwjson_debug.o \
./lwjson/src/lwjson/lwjson_stream.o 

C_DEPS += \
./lwjson/src/lwjson/lwjson.d \
./lwjson/src/lwjson/lwjson_debug.d \
./lwjson/src/lwjson/lwjson_stream.d 


# Each subdirectory must supply rules for building sources it contributes
lwjson/src/lwjson/%.o lwjson/src/lwjson/%.su lwjson/src/lwjson/%.cyclo: ../lwjson/src/lwjson/%.c lwjson/src/lwjson/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/leona/Documents/Marqueztronics/Projects/Reflow Oven/Firmware/Reflow-Oven-STM32/Drivers/lvgl" -I"C:/Users/leona/Documents/Marqueztronics/Projects/Reflow Oven/Firmware/Reflow-Oven-STM32/Drivers/tft" -I"C:/Users/leona/Documents/Marqueztronics/Projects/Reflow Oven/Firmware/Reflow-Oven-STM32/Drivers/touchpad" -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"C:/Users/leona/Documents/Marqueztronics/Projects/Reflow Oven/Firmware/Reflow-Oven-STM32/Drivers/SD" -I"C:/Users/leona/Documents/Marqueztronics/Projects/Reflow Oven/Firmware/Reflow-Oven-STM32/lwjson/src/include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lwjson-2f-src-2f-lwjson

clean-lwjson-2f-src-2f-lwjson:
	-$(RM) ./lwjson/src/lwjson/lwjson.cyclo ./lwjson/src/lwjson/lwjson.d ./lwjson/src/lwjson/lwjson.o ./lwjson/src/lwjson/lwjson.su ./lwjson/src/lwjson/lwjson_debug.cyclo ./lwjson/src/lwjson/lwjson_debug.d ./lwjson/src/lwjson/lwjson_debug.o ./lwjson/src/lwjson/lwjson_debug.su ./lwjson/src/lwjson/lwjson_stream.cyclo ./lwjson/src/lwjson/lwjson_stream.d ./lwjson/src/lwjson/lwjson_stream.o ./lwjson/src/lwjson/lwjson_stream.su

.PHONY: clean-lwjson-2f-src-2f-lwjson

