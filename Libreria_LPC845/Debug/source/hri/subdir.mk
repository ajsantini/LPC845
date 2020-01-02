################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/hri/HRI_UART.c 

OBJS += \
./source/hri/HRI_UART.o 

C_DEPS += \
./source/hri/HRI_UART.d 


# Each subdirectory must supply rules for building sources it contributes
source/hri/%.o: ../source/hri/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_LPC845M301JBD48 -DCPU_LPC845M301JBD48_cm0plus -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -DDEBUG -I../source -I"/home/augusto/Escritorio/InfoII/Libreria_AJS/Libreria_LPC845/includes" -I"/home/augusto/Escritorio/InfoII/Libreria_AJS/Libreria_LPC845/includes/hpl" -I"/home/augusto/Escritorio/InfoII/Libreria_AJS/Libreria_LPC845/includes/hri" -I../ -I"/home/augusto/Escritorio/InfoII/Libreria_AJS/Libreria_LPC845/source" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


