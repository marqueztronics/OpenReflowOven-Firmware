################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/lvgl/examples/widgets/msgbox/lv_example_msgbox_1.c \
../Drivers/lvgl/examples/widgets/msgbox/lv_example_msgbox_2.c 

OBJS += \
./Drivers/lvgl/examples/widgets/msgbox/lv_example_msgbox_1.o \
./Drivers/lvgl/examples/widgets/msgbox/lv_example_msgbox_2.o 

C_DEPS += \
./Drivers/lvgl/examples/widgets/msgbox/lv_example_msgbox_1.d \
./Drivers/lvgl/examples/widgets/msgbox/lv_example_msgbox_2.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/lvgl/examples/widgets/msgbox/%.o Drivers/lvgl/examples/widgets/msgbox/%.su Drivers/lvgl/examples/widgets/msgbox/%.cyclo: ../Drivers/lvgl/examples/widgets/msgbox/%.c Drivers/lvgl/examples/widgets/msgbox/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/leona/Documents/Marqueztronics/Projects/Reflow Oven/Firmware/Reflow-Oven-STM32/Drivers/lvgl" -I"C:/Users/leona/Documents/Marqueztronics/Projects/Reflow Oven/Firmware/Reflow-Oven-STM32/Drivers/tft" -I"C:/Users/leona/Documents/Marqueztronics/Projects/Reflow Oven/Firmware/Reflow-Oven-STM32/Drivers/touchpad" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-lvgl-2f-examples-2f-widgets-2f-msgbox

clean-Drivers-2f-lvgl-2f-examples-2f-widgets-2f-msgbox:
	-$(RM) ./Drivers/lvgl/examples/widgets/msgbox/lv_example_msgbox_1.cyclo ./Drivers/lvgl/examples/widgets/msgbox/lv_example_msgbox_1.d ./Drivers/lvgl/examples/widgets/msgbox/lv_example_msgbox_1.o ./Drivers/lvgl/examples/widgets/msgbox/lv_example_msgbox_1.su ./Drivers/lvgl/examples/widgets/msgbox/lv_example_msgbox_2.cyclo ./Drivers/lvgl/examples/widgets/msgbox/lv_example_msgbox_2.d ./Drivers/lvgl/examples/widgets/msgbox/lv_example_msgbox_2.o ./Drivers/lvgl/examples/widgets/msgbox/lv_example_msgbox_2.su

.PHONY: clean-Drivers-2f-lvgl-2f-examples-2f-widgets-2f-msgbox
