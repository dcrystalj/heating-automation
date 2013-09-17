################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../CMSIS/ST/STM32F4xx/Source/Templates/TASKING/cstart_thumb2.asm 

OBJS += \
./CMSIS/ST/STM32F4xx/Source/Templates/TASKING/cstart_thumb2.o 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/ST/STM32F4xx/Source/Templates/TASKING/%.o: ../CMSIS/ST/STM32F4xx/Source/Templates/TASKING/%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Assembler'
	arm-none-eabi-as -mthumb -mcpu=cortex-m4 -I/Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Libraries/CMSIS/Include -I/Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/inc -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/FreeRTOS" -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/FreeRTOS/include" -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/FreeRTOS/portable" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


