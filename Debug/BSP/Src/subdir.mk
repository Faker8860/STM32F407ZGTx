################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../BSP/Src/Bsp_AT24C02.c \
../BSP/Src/Bsp_Beep.c \
../BSP/Src/Bsp_KEY.c \
../BSP/Src/Bsp_LED.c \
../BSP/Src/Bsp_LightSensor.c \
../BSP/Src/Bsp_RS485.c \
../BSP/Src/Bsp_Servo.c 

OBJS += \
./BSP/Src/Bsp_AT24C02.o \
./BSP/Src/Bsp_Beep.o \
./BSP/Src/Bsp_KEY.o \
./BSP/Src/Bsp_LED.o \
./BSP/Src/Bsp_LightSensor.o \
./BSP/Src/Bsp_RS485.o \
./BSP/Src/Bsp_Servo.o 

C_DEPS += \
./BSP/Src/Bsp_AT24C02.d \
./BSP/Src/Bsp_Beep.d \
./BSP/Src/Bsp_KEY.d \
./BSP/Src/Bsp_LED.d \
./BSP/Src/Bsp_LightSensor.d \
./BSP/Src/Bsp_RS485.d \
./BSP/Src/Bsp_Servo.d 


# Each subdirectory must supply rules for building sources it contributes
BSP/Src/%.o BSP/Src/%.su BSP/Src/%.cyclo: ../BSP/Src/%.c BSP/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/modbus/tcp" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/modbus/ascii" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/modbus/tcp" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/modbus/rtu" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/modbus/include" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/functions" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/modbus" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus/port" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/FreeModbus" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/USER" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/BSP" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-BSP-2f-Src

clean-BSP-2f-Src:
	-$(RM) ./BSP/Src/Bsp_AT24C02.cyclo ./BSP/Src/Bsp_AT24C02.d ./BSP/Src/Bsp_AT24C02.o ./BSP/Src/Bsp_AT24C02.su ./BSP/Src/Bsp_Beep.cyclo ./BSP/Src/Bsp_Beep.d ./BSP/Src/Bsp_Beep.o ./BSP/Src/Bsp_Beep.su ./BSP/Src/Bsp_KEY.cyclo ./BSP/Src/Bsp_KEY.d ./BSP/Src/Bsp_KEY.o ./BSP/Src/Bsp_KEY.su ./BSP/Src/Bsp_LED.cyclo ./BSP/Src/Bsp_LED.d ./BSP/Src/Bsp_LED.o ./BSP/Src/Bsp_LED.su ./BSP/Src/Bsp_LightSensor.cyclo ./BSP/Src/Bsp_LightSensor.d ./BSP/Src/Bsp_LightSensor.o ./BSP/Src/Bsp_LightSensor.su ./BSP/Src/Bsp_RS485.cyclo ./BSP/Src/Bsp_RS485.d ./BSP/Src/Bsp_RS485.o ./BSP/Src/Bsp_RS485.su ./BSP/Src/Bsp_Servo.cyclo ./BSP/Src/Bsp_Servo.d ./BSP/Src/Bsp_Servo.o ./BSP/Src/Bsp_Servo.su

.PHONY: clean-BSP-2f-Src

