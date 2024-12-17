################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/lvgl/src/drivers/display/ili9341/lv_ili9341.c 

OBJS += \
./Drivers/lvgl/src/drivers/display/ili9341/lv_ili9341.o 

C_DEPS += \
./Drivers/lvgl/src/drivers/display/ili9341/lv_ili9341.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/lvgl/src/drivers/display/ili9341/%.o Drivers/lvgl/src/drivers/display/ili9341/%.su Drivers/lvgl/src/drivers/display/ili9341/%.cyclo: ../Drivers/lvgl/src/drivers/display/ili9341/%.c Drivers/lvgl/src/drivers/display/ili9341/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/leona/Documents/Marqueztronics/Projects/Reflow Oven/Firmware/Reflow-Oven-STM32/Drivers/lvgl" -I"C:/Users/leona/Documents/Marqueztronics/Projects/Reflow Oven/Firmware/Reflow-Oven-STM32/Drivers/tft" -I"C:/Users/leona/Documents/Marqueztronics/Projects/Reflow Oven/Firmware/Reflow-Oven-STM32/Drivers/touchpad" -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"C:/Users/leona/Documents/Marqueztronics/Projects/Reflow Oven/Firmware/Reflow-Oven-STM32/Drivers/SD" -I"C:/Users/leona/Documents/Marqueztronics/Projects/Reflow Oven/Firmware/Reflow-Oven-STM32/lwjson/src/include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-lvgl-2f-src-2f-drivers-2f-display-2f-ili9341

clean-Drivers-2f-lvgl-2f-src-2f-drivers-2f-display-2f-ili9341:
	-$(RM) ./Drivers/lvgl/src/drivers/display/ili9341/lv_ili9341.cyclo ./Drivers/lvgl/src/drivers/display/ili9341/lv_ili9341.d ./Drivers/lvgl/src/drivers/display/ili9341/lv_ili9341.o ./Drivers/lvgl/src/drivers/display/ili9341/lv_ili9341.su

.PHONY: clean-Drivers-2f-lvgl-2f-src-2f-drivers-2f-display-2f-ili9341

