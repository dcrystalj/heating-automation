################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LCD/AsciiLib.c \
../LCD/SSD1289.c \
../LCD/TouchPanel.c 

OBJS += \
./LCD/AsciiLib.o \
./LCD/SSD1289.o \
./LCD/TouchPanel.o 

C_DEPS += \
./LCD/AsciiLib.d \
./LCD/SSD1289.d \
./LCD/TouchPanel.d 


# Each subdirectory must supply rules for building sources it contributes
LCD/%.o: ../LCD/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-atollic-eabi-gcc -DSTM32F4XX -DUSE_STDPERIPH_DRIVER -I"C:\Users\Crystal\worskpace_stm32f_helios\STM32F4-FreeRTOS\CMSIS\Include" -I"C:\Users\Crystal\worskpace_stm32f_helios\STM32F4-FreeRTOS\CMSIS\ST\STM32F4xx\Include" -I"C:\Users\Crystal\worskpace_stm32f_helios\STM32F4-FreeRTOS\FreeRTOS" -I"C:\Users\Crystal\worskpace_stm32f_helios\STM32F4-FreeRTOS\FreeRTOS\portable" -I"C:\Users\Crystal\worskpace_stm32f_helios\STM32F4-FreeRTOS\FreeRTOS\include" -I"C:\Users\Crystal\worskpace_stm32f_helios\STM32F4-FreeRTOS\LCD" -I"C:\Users\Crystal\worskpace_stm32f_helios\STM32F4-FreeRTOS\STM32F4-Discovery" -I"C:\Users\Crystal\worskpace_stm32f_helios\STM32F4-FreeRTOS\STM32F4xx_StdPeriph_Driver\inc" -I"C:\Users\Crystal\worskpace_stm32f_helios\STM32F4-FreeRTOS\User" -I"C:\Users\Crystal\worskpace_stm32f_helios\STM32F4-FreeRTOS\Utils" -I"C:\Users\Crystal\worskpace_stm32f_helios\STM32F4-FreeRTOS\RFID" -I"C:\Users\Crystal\worskpace_stm32f_helios\STM32F4-FreeRTOS\I2C-DS1621" -Os -g3 -Wall -c -fmessage-length=0 -mthumb -mcpu=cortex-m4 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


