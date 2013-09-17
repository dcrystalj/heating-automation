################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_exti.c \
/Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_fsmc.c \
/Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c \
/Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c \
/Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_syscfg.c \
/Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_tim.c 

OBJS += \
./STM32F4xx_StdPeriph_Driver/stm32f4xx_exti.o \
./STM32F4xx_StdPeriph_Driver/stm32f4xx_fsmc.o \
./STM32F4xx_StdPeriph_Driver/stm32f4xx_gpio.o \
./STM32F4xx_StdPeriph_Driver/stm32f4xx_rcc.o \
./STM32F4xx_StdPeriph_Driver/stm32f4xx_syscfg.o \
./STM32F4xx_StdPeriph_Driver/stm32f4xx_tim.o 

C_DEPS += \
./STM32F4xx_StdPeriph_Driver/stm32f4xx_exti.d \
./STM32F4xx_StdPeriph_Driver/stm32f4xx_fsmc.d \
./STM32F4xx_StdPeriph_Driver/stm32f4xx_gpio.d \
./STM32F4xx_StdPeriph_Driver/stm32f4xx_rcc.d \
./STM32F4xx_StdPeriph_Driver/stm32f4xx_syscfg.d \
./STM32F4xx_StdPeriph_Driver/stm32f4xx_tim.d 


# Each subdirectory must supply rules for building sources it contributes
STM32F4xx_StdPeriph_Driver/stm32f4xx_exti.o: /Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_exti.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DSTM32F4XX=1 -DUSE_STDPERIPH_DRIVER=1 -I/Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Libraries/CMSIS/Include -I/Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Libraries/CMSIS/ST/STM32F4xx/Include -I/Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/inc -I/Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Utilities/STM32F4-Discovery -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/FreeRTOS" -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/FreeRTOS/include" -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/FreeRTOS/portable" -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/LCD" -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/User" -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/Utils" -Os -g3 -Wall -c -fmessage-length=0 -mthumb -mcpu=cortex-m4 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

STM32F4xx_StdPeriph_Driver/stm32f4xx_fsmc.o: /Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_fsmc.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DSTM32F4XX=1 -DUSE_STDPERIPH_DRIVER=1 -I/Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Libraries/CMSIS/Include -I/Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Libraries/CMSIS/ST/STM32F4xx/Include -I/Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/inc -I/Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Utilities/STM32F4-Discovery -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/FreeRTOS" -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/FreeRTOS/include" -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/FreeRTOS/portable" -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/LCD" -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/User" -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/Utils" -Os -g3 -Wall -c -fmessage-length=0 -mthumb -mcpu=cortex-m4 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

STM32F4xx_StdPeriph_Driver/stm32f4xx_gpio.o: /Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DSTM32F4XX=1 -DUSE_STDPERIPH_DRIVER=1 -I/Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Libraries/CMSIS/Include -I/Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Libraries/CMSIS/ST/STM32F4xx/Include -I/Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/inc -I/Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Utilities/STM32F4-Discovery -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/FreeRTOS" -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/FreeRTOS/include" -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/FreeRTOS/portable" -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/LCD" -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/User" -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/Utils" -Os -g3 -Wall -c -fmessage-length=0 -mthumb -mcpu=cortex-m4 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

STM32F4xx_StdPeriph_Driver/stm32f4xx_rcc.o: /Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DSTM32F4XX=1 -DUSE_STDPERIPH_DRIVER=1 -I/Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Libraries/CMSIS/Include -I/Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Libraries/CMSIS/ST/STM32F4xx/Include -I/Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/inc -I/Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Utilities/STM32F4-Discovery -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/FreeRTOS" -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/FreeRTOS/include" -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/FreeRTOS/portable" -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/LCD" -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/User" -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/Utils" -Os -g3 -Wall -c -fmessage-length=0 -mthumb -mcpu=cortex-m4 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

STM32F4xx_StdPeriph_Driver/stm32f4xx_syscfg.o: /Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_syscfg.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DSTM32F4XX=1 -DUSE_STDPERIPH_DRIVER=1 -I/Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Libraries/CMSIS/Include -I/Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Libraries/CMSIS/ST/STM32F4xx/Include -I/Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/inc -I/Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Utilities/STM32F4-Discovery -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/FreeRTOS" -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/FreeRTOS/include" -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/FreeRTOS/portable" -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/LCD" -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/User" -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/Utils" -Os -g3 -Wall -c -fmessage-length=0 -mthumb -mcpu=cortex-m4 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

STM32F4xx_StdPeriph_Driver/stm32f4xx_tim.o: /Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_tim.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DSTM32F4XX=1 -DUSE_STDPERIPH_DRIVER=1 -I/Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Libraries/CMSIS/Include -I/Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Libraries/CMSIS/ST/STM32F4xx/Include -I/Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/inc -I/Users/patricio/Documents/stm32f4-discovery-projects/STM32F4-Discovery_FW_V1.1.0/Utilities/STM32F4-Discovery -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/FreeRTOS" -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/FreeRTOS/include" -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/FreeRTOS/portable" -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/LCD" -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/User" -I"/Users/patricio/Documents/stm32f4-discovery-projects/workspace/STM32F4-FreeRTOS/Utils" -Os -g3 -Wall -c -fmessage-length=0 -mthumb -mcpu=cortex-m4 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


