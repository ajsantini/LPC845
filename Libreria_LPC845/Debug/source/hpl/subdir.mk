################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/hpl/HPL_ADC.c \
../source/hpl/HPL_CTIMER.c \
../source/hpl/HPL_DAC.c \
../source/hpl/HPL_GPIO.c \
../source/hpl/HPL_IOCON.c \
../source/hpl/HPL_NVIC.c \
../source/hpl/HPL_PININT.c \
../source/hpl/HPL_SPI.c \
../source/hpl/HPL_SWM.c \
../source/hpl/HPL_SYSCON.c \
../source/hpl/HPL_SYSTICK.c \
../source/hpl/HPL_UART.c \
../source/hpl/HPL_WKT.c 

OBJS += \
./source/hpl/HPL_ADC.o \
./source/hpl/HPL_CTIMER.o \
./source/hpl/HPL_DAC.o \
./source/hpl/HPL_GPIO.o \
./source/hpl/HPL_IOCON.o \
./source/hpl/HPL_NVIC.o \
./source/hpl/HPL_PININT.o \
./source/hpl/HPL_SPI.o \
./source/hpl/HPL_SWM.o \
./source/hpl/HPL_SYSCON.o \
./source/hpl/HPL_SYSTICK.o \
./source/hpl/HPL_UART.o \
./source/hpl/HPL_WKT.o 

C_DEPS += \
./source/hpl/HPL_ADC.d \
./source/hpl/HPL_CTIMER.d \
./source/hpl/HPL_DAC.d \
./source/hpl/HPL_GPIO.d \
./source/hpl/HPL_IOCON.d \
./source/hpl/HPL_NVIC.d \
./source/hpl/HPL_PININT.d \
./source/hpl/HPL_SPI.d \
./source/hpl/HPL_SWM.d \
./source/hpl/HPL_SYSCON.d \
./source/hpl/HPL_SYSTICK.d \
./source/hpl/HPL_UART.d \
./source/hpl/HPL_WKT.d 


# Each subdirectory must supply rules for building sources it contributes
source/hpl/%.o: ../source/hpl/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_LPC845M301JBD48 -DCPU_LPC845M301JBD48_cm0plus -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -DDEBUG -I"/home/augusto/Desktop/InfoII/Libreria_LPC845/LPC845/Libreria_LPC845/includes" -I"/home/augusto/Desktop/InfoII/Libreria_LPC845/LPC845/Libreria_LPC845/includes/hal" -I"/home/augusto/Desktop/InfoII/Libreria_LPC845/LPC845/Libreria_LPC845/includes/infotronic" -I"/home/augusto/Desktop/InfoII/Libreria_LPC845/LPC845/Libreria_LPC845/includes/hpl" -I"/home/augusto/Desktop/InfoII/Libreria_LPC845/LPC845/Libreria_LPC845/includes/hri" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mcpu=cortex-m0 -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


