################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/lvgl/src/widgets/win/lv_win.c 

OBJS += \
./Drivers/lvgl/src/widgets/win/lv_win.o 

C_DEPS += \
./Drivers/lvgl/src/widgets/win/lv_win.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/lvgl/src/widgets/win/%.o Drivers/lvgl/src/widgets/win/%.su Drivers/lvgl/src/widgets/win/%.cyclo: ../Drivers/lvgl/src/widgets/win/%.c Drivers/lvgl/src/widgets/win/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/leona/Documents/Marqueztronics/Projects/Reflow Oven/Firmware/Reflow-Oven-STM32/Drivers/lvgl" -I"C:/Users/leona/Documents/Marqueztronics/Projects/Reflow Oven/Firmware/Reflow-Oven-STM32/Drivers/tft" -I"C:/Users/leona/Documents/Marqueztronics/Projects/Reflow Oven/Firmware/Reflow-Oven-STM32/Drivers/touchpad" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-lvgl-2f-src-2f-widgets-2f-win

clean-Drivers-2f-lvgl-2f-src-2f-widgets-2f-win:
	-$(RM) ./Drivers/lvgl/src/widgets/win/lv_win.cyclo ./Drivers/lvgl/src/widgets/win/lv_win.d ./Drivers/lvgl/src/widgets/win/lv_win.o ./Drivers/lvgl/src/widgets/win/lv_win.su

.PHONY: clean-Drivers-2f-lvgl-2f-src-2f-widgets-2f-win
