################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c 

OBJS += \
./CMSIS/ST/STM32F4xx/Source/Templates/system_stm32f4xx.o 

C_DEPS += \
./CMSIS/ST/STM32F4xx/Source/Templates/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/ST/STM32F4xx/Source/Templates/%.o: ../CMSIS/ST/STM32F4xx/Source/Templates/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DSTM32F4XX -DUSE_STDPERIPH_DRIVER -I/Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Libraries/CMSIS/Include -I/Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Libraries/CMSIS/ST/STM32F4xx/Include -I/Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/inc -I/Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Utilities/STM32F4-Discovery -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/FreeRTOS" -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/FreeRTOS/include" -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/FreeRTOS/portable" -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/LCD" -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/User" -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/Utils" -Os -g3 -Wall -c -fmessage-length=0 -mthumb -mcpu=cortex-m4 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


