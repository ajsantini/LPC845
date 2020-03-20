################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/mtb.c 

OBJS += \
./source/mtb.o 

C_DEPS += \
./source/mtb.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_LPC845M301JBD48 -DCPU_LPC845M301JBD48_cm0plus -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -DDEBUG -I"/home/augusto/Desktop/InfoII/Libreria_LPC845/LPC845/Libreria_LPC845/includes" -I"/home/augusto/Desktop/InfoII/Libreria_LPC845/LPC845/Libreria_LPC845/includes/hal" -I"/home/augusto/Desktop/InfoII/Libreria_LPC845/LPC845/Libreria_LPC845/includes/infotronic" -I"/home/augusto/Desktop/InfoII/Libreria_LPC845/LPC845/Libreria_LPC845/includes/hpl" -I"/home/augusto/Desktop/InfoII/Libreria_LPC845/LPC845/Libreria_LPC845/includes/hri" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mcpu=cortex-m0 -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


