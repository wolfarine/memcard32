################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../icode/oled/oled.c 

OBJS += \
./icode/oled/oled.o 

C_DEPS += \
./icode/oled/oled.d 


# Each subdirectory must supply rules for building sources it contributes
icode/oled/%.o icode/oled/%.su: ../icode/oled/%.c icode/oled/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-icode-2f-oled

clean-icode-2f-oled:
	-$(RM) ./icode/oled/oled.d ./icode/oled/oled.o ./icode/oled/oled.su

.PHONY: clean-icode-2f-oled

