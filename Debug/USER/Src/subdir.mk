################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../USER/Src/APP_Config.c \
../USER/Src/Command.c 

OBJS += \
./USER/Src/APP_Config.o \
./USER/Src/Command.o 

C_DEPS += \
./USER/Src/APP_Config.d \
./USER/Src/Command.d 


# Each subdirectory must supply rules for building sources it contributes
USER/Src/%.o USER/Src/%.su USER/Src/%.cyclo: ../USER/Src/%.c USER/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/modbus/tcp" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/modbus/ascii" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/modbus/tcp" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/modbus/rtu" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/modbus/include" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/functions" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/modbus" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/port" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/USER" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/BSP" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-USER-2f-Src

clean-USER-2f-Src:
	-$(RM) ./USER/Src/APP_Config.cyclo ./USER/Src/APP_Config.d ./USER/Src/APP_Config.o ./USER/Src/APP_Config.su ./USER/Src/Command.cyclo ./USER/Src/Command.d ./USER/Src/Command.o ./USER/Src/Command.su

.PHONY: clean-USER-2f-Src

