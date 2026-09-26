################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeModbus/modbus/mb.c 

OBJS += \
./FreeModbus/modbus/mb.o 

C_DEPS += \
./FreeModbus/modbus/mb.d 


# Each subdirectory must supply rules for building sources it contributes
FreeModbus/modbus/%.o FreeModbus/modbus/%.su FreeModbus/modbus/%.cyclo: ../FreeModbus/modbus/%.c FreeModbus/modbus/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/modbus/tcp" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/modbus/ascii" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/modbus/tcp" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/modbus/rtu" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/modbus/include" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/functions" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/modbus" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/port" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/USER" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/BSP" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-FreeModbus-2f-modbus

clean-FreeModbus-2f-modbus:
	-$(RM) ./FreeModbus/modbus/mb.cyclo ./FreeModbus/modbus/mb.d ./FreeModbus/modbus/mb.o ./FreeModbus/modbus/mb.su

.PHONY: clean-FreeModbus-2f-modbus

