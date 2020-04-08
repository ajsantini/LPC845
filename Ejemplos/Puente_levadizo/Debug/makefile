################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include Modulos/subdir.mk
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
all: Puente_levadizo.axf

# Tool invocations
Puente_levadizo.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"/home/augusto/Desktop/InfoII/Libreria_LPC845/LPC845/Libreria_LPC845/Debug" -Xlinker -Map="Puente_levadizo.map" -Xlinker --cref -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m0 -mthumb -T "Puente_levadizo_Debug.ld" -o "Puente_levadizo.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) Puente_levadizo.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "Puente_levadizo.axf"; # arm-none-eabi-objcopy -v -O binary "Puente_levadizo.axf" "Puente_levadizo.bin" ; # checksum -p LPC845 -d "Puente_levadizo.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
