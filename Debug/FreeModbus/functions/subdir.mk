################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeModbus/functions/mbfunccoils.c \
../FreeModbus/functions/mbfuncdiag.c \
../FreeModbus/functions/mbfuncdisc.c \
../FreeModbus/functions/mbfuncfile.c \
../FreeModbus/functions/mbfuncholding.c \
../FreeModbus/functions/mbfuncinput.c \
../FreeModbus/functions/mbfuncother.c \
../FreeModbus/functions/mbutils.c 

OBJS += \
./FreeModbus/functions/mbfunccoils.o \
./FreeModbus/functions/mbfuncdiag.o \
./FreeModbus/functions/mbfuncdisc.o \
./FreeModbus/functions/mbfuncfile.o \
./FreeModbus/functions/mbfuncholding.o \
./FreeModbus/functions/mbfuncinput.o \
./FreeModbus/functions/mbfuncother.o \
./FreeModbus/functions/mbutils.o 

C_DEPS += \
./FreeModbus/functions/mbfunccoils.d \
./FreeModbus/functions/mbfuncdiag.d \
./FreeModbus/functions/mbfuncdisc.d \
./FreeModbus/functions/mbfuncfile.d \
./FreeModbus/functions/mbfuncholding.d \
./FreeModbus/functions/mbfuncinput.d \
./FreeModbus/functions/mbfuncother.d \
./FreeModbus/functions/mbutils.d 


# Each subdirectory must supply rules for building sources it contributes
FreeModbus/functions/%.o FreeModbus/functions/%.su FreeModbus/functions/%.cyclo: ../FreeModbus/functions/%.c FreeModbus/functions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/modbus/tcp" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/modbus/ascii" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/modbus/tcp" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/modbus/rtu" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/modbus/include" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/functions" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/modbus" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/port" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/USER" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/BSP" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-FreeModbus-2f-functions

clean-FreeModbus-2f-functions:
	-$(RM) ./FreeModbus/functions/mbfunccoils.cyclo ./FreeModbus/functions/mbfunccoils.d ./FreeModbus/functions/mbfunccoils.o ./FreeModbus/functions/mbfunccoils.su ./FreeModbus/functions/mbfuncdiag.cyclo ./FreeModbus/functions/mbfuncdiag.d ./FreeModbus/functions/mbfuncdiag.o ./FreeModbus/functions/mbfuncdiag.su ./FreeModbus/functions/mbfuncdisc.cyclo ./FreeModbus/functions/mbfuncdisc.d ./FreeModbus/functions/mbfuncdisc.o ./FreeModbus/functions/mbfuncdisc.su ./FreeModbus/functions/mbfuncfile.cyclo ./FreeModbus/functions/mbfuncfile.d ./FreeModbus/functions/mbfuncfile.o ./FreeModbus/functions/mbfuncfile.su ./FreeModbus/functions/mbfuncholding.cyclo ./FreeModbus/functions/mbfuncholding.d ./FreeModbus/functions/mbfuncholding.o ./FreeModbus/functions/mbfuncholding.su ./FreeModbus/functions/mbfuncinput.cyclo ./FreeModbus/functions/mbfuncinput.d ./FreeModbus/functions/mbfuncinput.o ./FreeModbus/functions/mbfuncinput.su ./FreeModbus/functions/mbfuncother.cyclo ./FreeModbus/functions/mbfuncother.d ./FreeModbus/functions/mbfuncother.o ./FreeModbus/functions/mbfuncother.su ./FreeModbus/functions/mbutils.cyclo ./FreeModbus/functions/mbutils.d ./FreeModbus/functions/mbutils.o ./FreeModbus/functions/mbutils.su

.PHONY: clean-FreeModbus-2f-functions

