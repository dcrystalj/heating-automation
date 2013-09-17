################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include startup/subdir.mk
-include Utils/subdir.mk
-include User/subdir.mk
-include STM32F4xx_StdPeriph_Driver/src/subdir.mk
-include STM32F4-Discovery/subdir.mk
-include RFID/subdir.mk
-include LCD/subdir.mk
-include I2C-DS1621/subdir.mk
-include FreeRTOS/subdir.mk
-include FreeRTOS/portable/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: STM32F4-FreeRTOS

# Tool invocations
STM32F4-FreeRTOS: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross G++ Linker'
	arm-atollic-eabi-gcc -T "C:\Users\Crystal\worskpace_stm32f_helios/STM32F4-FreeRTOS/startup/stm32_flash.ld" -mthumb -mcpu=cortex-m4 -Wl,-N -Wl,-M -Wl,-Map=linker.map -Wl,-cref -Wl,--gc-sections -o "STM32F4-FreeRTOS" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(C++_DEPS)$(OBJS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) STM32F4-FreeRTOS
	-@echo ' '

post-build:
	-@echo 'this will creat binary, which we can upload with the STLink Utility'
	-arm-none-eabi-objcopy -S  -O binary  "STM32F4-FreeRTOS" "STM32F4-FreeRTOS.bin"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
