################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/lvgl/src/others/gridnav/lv_gridnav.c 

OBJS += \
./Drivers/lvgl/src/others/gridnav/lv_gridnav.o 

C_DEPS += \
./Drivers/lvgl/src/others/gridnav/lv_gridnav.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/lvgl/src/others/gridnav/%.o Drivers/lvgl/src/others/gridnav/%.su Drivers/lvgl/src/others/gridnav/%.cyclo: ../Drivers/lvgl/src/others/gridnav/%.c Drivers/lvgl/src/others/gridnav/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/leona/Documents/Marqueztronics/Projects/Reflow Oven/Firmware/Reflow-Oven-STM32/Drivers/lvgl" -I"C:/Users/leona/Documents/Marqueztronics/Projects/Reflow Oven/Firmware/Reflow-Oven-STM32/Drivers/tft" -I"C:/Users/leona/Documents/Marqueztronics/Projects/Reflow Oven/Firmware/Reflow-Oven-STM32/Drivers/touchpad" -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"C:/Users/leona/Documents/Marqueztronics/Projects/Reflow Oven/Firmware/Reflow-Oven-STM32/Drivers/SD" -I"C:/Users/leona/Documents/Marqueztronics/Projects/Reflow Oven/Firmware/Reflow-Oven-STM32/lwjson/src/include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-lvgl-2f-src-2f-others-2f-gridnav

clean-Drivers-2f-lvgl-2f-src-2f-others-2f-gridnav:
	-$(RM) ./Drivers/lvgl/src/others/gridnav/lv_gridnav.cyclo ./Drivers/lvgl/src/others/gridnav/lv_gridnav.d ./Drivers/lvgl/src/others/gridnav/lv_gridnav.o ./Drivers/lvgl/src/others/gridnav/lv_gridnav.su

.PHONY: clean-Drivers-2f-lvgl-2f-src-2f-others-2f-gridnav

