################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../Drivers/lvgl/src/draw/sw/blend/neon/lv_blend_neon.S 

OBJS += \
./Drivers/lvgl/src/draw/sw/blend/neon/lv_blend_neon.o 

S_UPPER_DEPS += \
./Drivers/lvgl/src/draw/sw/blend/neon/lv_blend_neon.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/lvgl/src/draw/sw/blend/neon/%.o: ../Drivers/lvgl/src/draw/sw/blend/neon/%.S Drivers/lvgl/src/draw/sw/blend/neon/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -I"C:/Users/leona/Documents/Marqueztronics/Projects/Reflow Oven/Firmware/Reflow-Oven-STM32/Drivers/lvgl" -I"C:/Users/leona/Documents/Marqueztronics/Projects/Reflow Oven/Firmware/Reflow-Oven-STM32/Drivers/tft" -I"C:/Users/leona/Documents/Marqueztronics/Projects/Reflow Oven/Firmware/Reflow-Oven-STM32/Drivers/touchpad" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Drivers-2f-lvgl-2f-src-2f-draw-2f-sw-2f-blend-2f-neon

clean-Drivers-2f-lvgl-2f-src-2f-draw-2f-sw-2f-blend-2f-neon:
	-$(RM) ./Drivers/lvgl/src/draw/sw/blend/neon/lv_blend_neon.d ./Drivers/lvgl/src/draw/sw/blend/neon/lv_blend_neon.o

.PHONY: clean-Drivers-2f-lvgl-2f-src-2f-draw-2f-sw-2f-blend-2f-neon

