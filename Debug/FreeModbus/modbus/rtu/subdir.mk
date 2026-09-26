################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeModbus/modbus/rtu/mbcrc.c \
../FreeModbus/modbus/rtu/mbrtu.c 

OBJS += \
./FreeModbus/modbus/rtu/mbcrc.o \
./FreeModbus/modbus/rtu/mbrtu.o 

C_DEPS += \
./FreeModbus/modbus/rtu/mbcrc.d \
./FreeModbus/modbus/rtu/mbrtu.d 


# Each subdirectory must supply rules for building sources it contributes
FreeModbus/modbus/rtu/%.o FreeModbus/modbus/rtu/%.su FreeModbus/modbus/rtu/%.cyclo: ../FreeModbus/modbus/rtu/%.c FreeModbus/modbus/rtu/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/modbus/tcp" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/modbus/ascii" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/modbus/tcp" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/modbus/rtu" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/modbus/include" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/functions" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/modbus" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/port" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/USER" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/BSP" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-FreeModbus-2f-modbus-2f-rtu

clean-FreeModbus-2f-modbus-2f-rtu:
	-$(RM) ./FreeModbus/modbus/rtu/mbcrc.cyclo ./FreeModbus/modbus/rtu/mbcrc.d ./FreeModbus/modbus/rtu/mbcrc.o ./FreeModbus/modbus/rtu/mbcrc.su ./FreeModbus/modbus/rtu/mbrtu.cyclo ./FreeModbus/modbus/rtu/mbrtu.d ./FreeModbus/modbus/rtu/mbrtu.o ./FreeModbus/modbus/rtu/mbrtu.su

.PHONY: clean-FreeModbus-2f-modbus-2f-rtu

