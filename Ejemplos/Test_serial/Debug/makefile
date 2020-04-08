################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: Test_serial.axf

# Tool invocations
Test_serial.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"/home/augusto/Desktop/InfoII/Libreria_LPC845/LPC845/Libreria_LPC845/Debug" -Xlinker -Map="Test_serial.map" -Xlinker --cref -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m0 -mthumb -T "Test_serial_Debug.ld" -o "Test_serial.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) Test_serial.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "Test_serial.axf"; # arm-none-eabi-objcopy -v -O binary "Test_serial.axf" "Test_serial.bin" ; # checksum -p LPC845 -d "Test_serial.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
