################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/lvgl/demos/transform/lv_demo_transform.c 

OBJS += \
./Drivers/lvgl/demos/transform/lv_demo_transform.o 

C_DEPS += \
./Drivers/lvgl/demos/transform/lv_demo_transform.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/lvgl/demos/transform/%.o Drivers/lvgl/demos/transform/%.su Drivers/lvgl/demos/transform/%.cyclo: ../Drivers/lvgl/demos/transform/%.c Drivers/lvgl/demos/transform/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/leona/Documents/Marqueztronics/Projects/Reflow Oven/Firmware/Reflow-Oven-STM32/Drivers/lvgl" -I"C:/Users/leona/Documents/Marqueztronics/Projects/Reflow Oven/Firmware/Reflow-Oven-STM32/Drivers/tft" -I"C:/Users/leona/Documents/Marqueztronics/Projects/Reflow Oven/Firmware/Reflow-Oven-STM32/Drivers/touchpad" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-lvgl-2f-demos-2f-transform

clean-Drivers-2f-lvgl-2f-demos-2f-transform:
	-$(RM) ./Drivers/lvgl/demos/transform/lv_demo_transform.cyclo ./Drivers/lvgl/demos/transform/lv_demo_transform.d ./Drivers/lvgl/demos/transform/lv_demo_transform.o ./Drivers/lvgl/demos/transform/lv_demo_transform.su

.PHONY: clean-Drivers-2f-lvgl-2f-demos-2f-transform

