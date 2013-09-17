################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../User/main.o 

C_SRCS += \
../User/main.c \
../User/stm32f4xx_it.c \
../User/system_stm32f4xx.c 

OBJS += \
./User/main.o \
./User/stm32f4xx_it.o \
./User/system_stm32f4xx.o 

C_DEPS += \
./User/main.d \
./User/stm32f4xx_it.d \
./User/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-atollic-eabi-gcc -DSTM32F4XX -DUSE_STDPERIPH_DRIVER -I"C:\Users\Crystal\worskpace_stm32f_helios\STM32F4-FreeRTOS\CMSIS\Include" -I"C:\Users\Crystal\worskpace_stm32f_helios\STM32F4-FreeRTOS\CMSIS\ST\STM32F4xx\Include" -I"C:\Users\Crystal\worskpace_stm32f_helios\STM32F4-FreeRTOS\FreeRTOS" -I"C:\Users\Crystal\worskpace_stm32f_helios\STM32F4-FreeRTOS\FreeRTOS\portable" -I"C:\Users\Crystal\worskpace_stm32f_helios\STM32F4-FreeRTOS\FreeRTOS\include" -I"C:\Users\Crystal\worskpace_stm32f_helios\STM32F4-FreeRTOS\LCD" -I"C:\Users\Crystal\worskpace_stm32f_helios\STM32F4-FreeRTOS\STM32F4-Discovery" -I"C:\Users\Crystal\worskpace_stm32f_helios\STM32F4-FreeRTOS\STM32F4xx_StdPeriph_Driver\inc" -I"C:\Users\Crystal\worskpace_stm32f_helios\STM32F4-FreeRTOS\User" -I"C:\Users\Crystal\worskpace_stm32f_helios\STM32F4-FreeRTOS\Utils" -I"C:\Users\Crystal\worskpace_stm32f_helios\STM32F4-FreeRTOS\RFID" -I"C:\Users\Crystal\worskpace_stm32f_helios\STM32F4-FreeRTOS\I2C-DS1621" -Os -g3 -Wall -c -fmessage-length=0 -mthumb -mcpu=cortex-m4 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


