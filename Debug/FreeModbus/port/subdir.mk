################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeModbus/port/port.c \
../FreeModbus/port/portevent.c \
../FreeModbus/port/portserial.c \
../FreeModbus/port/porttimer.c 

OBJS += \
./FreeModbus/port/port.o \
./FreeModbus/port/portevent.o \
./FreeModbus/port/portserial.o \
./FreeModbus/port/porttimer.o 

C_DEPS += \
./FreeModbus/port/port.d \
./FreeModbus/port/portevent.d \
./FreeModbus/port/portserial.d \
./FreeModbus/port/porttimer.d 


# Each subdirectory must supply rules for building sources it contributes
FreeModbus/port/%.o FreeModbus/port/%.su FreeModbus/port/%.cyclo: ../FreeModbus/port/%.c FreeModbus/port/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/modbus/tcp" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/modbus/ascii" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/modbus/tcp" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/modbus/rtu" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/modbus/include" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/functions" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/modbus" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/port" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/USER" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/BSP" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-FreeModbus-2f-port

clean-FreeModbus-2f-port:
	-$(RM) ./FreeModbus/port/port.cyclo ./FreeModbus/port/port.d ./FreeModbus/port/port.o ./FreeModbus/port/port.su ./FreeModbus/port/portevent.cyclo ./FreeModbus/port/portevent.d ./FreeModbus/port/portevent.o ./FreeModbus/port/portevent.su ./FreeModbus/port/portserial.cyclo ./FreeModbus/port/portserial.d ./FreeModbus/port/portserial.o ./FreeModbus/port/portserial.su ./FreeModbus/port/porttimer.cyclo ./FreeModbus/port/porttimer.d ./FreeModbus/port/porttimer.o ./FreeModbus/port/porttimer.su

.PHONY: clean-FreeModbus-2f-port

