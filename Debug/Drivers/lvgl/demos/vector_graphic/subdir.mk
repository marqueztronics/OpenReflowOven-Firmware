################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/lvgl/demos/vector_graphic/lv_demo_vector_graphic.c 

OBJS += \
./Drivers/lvgl/demos/vector_graphic/lv_demo_vector_graphic.o 

C_DEPS += \
./Drivers/lvgl/demos/vector_graphic/lv_demo_vector_graphic.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/lvgl/demos/vector_graphic/%.o Drivers/lvgl/demos/vector_graphic/%.su Drivers/lvgl/demos/vector_graphic/%.cyclo: ../Drivers/lvgl/demos/vector_graphic/%.c Drivers/lvgl/demos/vector_graphic/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/leona/Documents/Marqueztronics/Projects/Reflow Oven/Firmware/Reflow-Oven-STM32/Drivers/lvgl" -I"C:/Users/leona/Documents/Marqueztronics/Projects/Reflow Oven/Firmware/Reflow-Oven-STM32/Drivers/tft" -I"C:/Users/leona/Documents/Marqueztronics/Projects/Reflow Oven/Firmware/Reflow-Oven-STM32/Drivers/touchpad" -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"C:/Users/leona/Documents/Marqueztronics/Projects/Reflow Oven/Firmware/Reflow-Oven-STM32/Drivers/SD" -I"C:/Users/leona/Documents/Marqueztronics/Projects/Reflow Oven/Firmware/Reflow-Oven-STM32/lwjson/src/include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-lvgl-2f-demos-2f-vector_graphic

clean-Drivers-2f-lvgl-2f-demos-2f-vector_graphic:
	-$(RM) ./Drivers/lvgl/demos/vector_graphic/lv_demo_vector_graphic.cyclo ./Drivers/lvgl/demos/vector_graphic/lv_demo_vector_graphic.d ./Drivers/lvgl/demos/vector_graphic/lv_demo_vector_graphic.o ./Drivers/lvgl/demos/vector_graphic/lv_demo_vector_graphic.su

.PHONY: clean-Drivers-2f-lvgl-2f-demos-2f-vector_graphic

