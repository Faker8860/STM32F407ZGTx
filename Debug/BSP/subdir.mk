################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../BSP/Bsp_Beep.c \
../BSP/Bsp_KEY.c \
../BSP/Bsp_LED.c \
../BSP/Bsp_Servo.c 

OBJS += \
./BSP/Bsp_Beep.o \
./BSP/Bsp_KEY.o \
./BSP/Bsp_LED.o \
./BSP/Bsp_Servo.o 

C_DEPS += \
./BSP/Bsp_Beep.d \
./BSP/Bsp_KEY.d \
./BSP/Bsp_LED.d \
./BSP/Bsp_Servo.d 


# Each subdirectory must supply rules for building sources it contributes
BSP/%.o BSP/%.su BSP/%.cyclo: ../BSP/%.c BSP/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/USER" -I"D:/CompanyFiles/Code/Study_Code/STM32F407ZGTx/BSP" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-BSP

clean-BSP:
	-$(RM) ./BSP/Bsp_Beep.cyclo ./BSP/Bsp_Beep.d ./BSP/Bsp_Beep.o ./BSP/Bsp_Beep.su ./BSP/Bsp_KEY.cyclo ./BSP/Bsp_KEY.d ./BSP/Bsp_KEY.o ./BSP/Bsp_KEY.su ./BSP/Bsp_LED.cyclo ./BSP/Bsp_LED.d ./BSP/Bsp_LED.o ./BSP/Bsp_LED.su ./BSP/Bsp_Servo.cyclo ./BSP/Bsp_Servo.d ./BSP/Bsp_Servo.o ./BSP/Bsp_Servo.su

.PHONY: clean-BSP

