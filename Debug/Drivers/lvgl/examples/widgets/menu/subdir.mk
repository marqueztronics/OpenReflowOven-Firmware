################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/lvgl/examples/widgets/menu/lv_example_menu_1.c \
../Drivers/lvgl/examples/widgets/menu/lv_example_menu_2.c \
../Drivers/lvgl/examples/widgets/menu/lv_example_menu_3.c \
../Drivers/lvgl/examples/widgets/menu/lv_example_menu_4.c \
../Drivers/lvgl/examples/widgets/menu/lv_example_menu_5.c 

OBJS += \
./Drivers/lvgl/examples/widgets/menu/lv_example_menu_1.o \
./Drivers/lvgl/examples/widgets/menu/lv_example_menu_2.o \
./Drivers/lvgl/examples/widgets/menu/lv_example_menu_3.o \
./Drivers/lvgl/examples/widgets/menu/lv_example_menu_4.o \
./Drivers/lvgl/examples/widgets/menu/lv_example_menu_5.o 

C_DEPS += \
./Drivers/lvgl/examples/widgets/menu/lv_example_menu_1.d \
./Drivers/lvgl/examples/widgets/menu/lv_example_menu_2.d \
./Drivers/lvgl/examples/widgets/menu/lv_example_menu_3.d \
./Drivers/lvgl/examples/widgets/menu/lv_example_menu_4.d \
./Drivers/lvgl/examples/widgets/menu/lv_example_menu_5.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/lvgl/examples/widgets/menu/%.o Drivers/lvgl/examples/widgets/menu/%.su Drivers/lvgl/examples/widgets/menu/%.cyclo: ../Drivers/lvgl/examples/widgets/menu/%.c Drivers/lvgl/examples/widgets/menu/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/leona/Documents/Marqueztronics/Projects/Reflow Oven/Firmware/Reflow-Oven-STM32/Drivers/lvgl" -I"C:/Users/leona/Documents/Marqueztronics/Projects/Reflow Oven/Firmware/Reflow-Oven-STM32/Drivers/tft" -I"C:/Users/leona/Documents/Marqueztronics/Projects/Reflow Oven/Firmware/Reflow-Oven-STM32/Drivers/touchpad" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-lvgl-2f-examples-2f-widgets-2f-menu

clean-Drivers-2f-lvgl-2f-examples-2f-widgets-2f-menu:
	-$(RM) ./Drivers/lvgl/examples/widgets/menu/lv_example_menu_1.cyclo ./Drivers/lvgl/examples/widgets/menu/lv_example_menu_1.d ./Drivers/lvgl/examples/widgets/menu/lv_example_menu_1.o ./Drivers/lvgl/examples/widgets/menu/lv_example_menu_1.su ./Drivers/lvgl/examples/widgets/menu/lv_example_menu_2.cyclo ./Drivers/lvgl/examples/widgets/menu/lv_example_menu_2.d ./Drivers/lvgl/examples/widgets/menu/lv_example_menu_2.o ./Drivers/lvgl/examples/widgets/menu/lv_example_menu_2.su ./Drivers/lvgl/examples/widgets/menu/lv_example_menu_3.cyclo ./Drivers/lvgl/examples/widgets/menu/lv_example_menu_3.d ./Drivers/lvgl/examples/widgets/menu/lv_example_menu_3.o ./Drivers/lvgl/examples/widgets/menu/lv_example_menu_3.su ./Drivers/lvgl/examples/widgets/menu/lv_example_menu_4.cyclo ./Drivers/lvgl/examples/widgets/menu/lv_example_menu_4.d ./Drivers/lvgl/examples/widgets/menu/lv_example_menu_4.o ./Drivers/lvgl/examples/widgets/menu/lv_example_menu_4.su ./Drivers/lvgl/examples/widgets/menu/lv_example_menu_5.cyclo ./Drivers/lvgl/examples/widgets/menu/lv_example_menu_5.d ./Drivers/lvgl/examples/widgets/menu/lv_example_menu_5.o ./Drivers/lvgl/examples/widgets/menu/lv_example_menu_5.su

.PHONY: clean-Drivers-2f-lvgl-2f-examples-2f-widgets-2f-menu

