################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Modulos/motores.c \
../Modulos/semaforos.c \
../Modulos/sensores_presencia.c \
../Modulos/sensores_tope.c 

OBJS += \
./Modulos/motores.o \
./Modulos/semaforos.o \
./Modulos/sensores_presencia.o \
./Modulos/sensores_tope.o 

C_DEPS += \
./Modulos/motores.d \
./Modulos/semaforos.d \
./Modulos/sensores_presencia.d \
./Modulos/sensores_tope.d 


# Each subdirectory must supply rules for building sources it contributes
Modulos/%.o: ../Modulos/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M0PLUS -D__MTB_DISABLE -D__MTB_BUFFER_SIZE=256 -D__LPC84X__ -D__REDLIB__ -I"/home/augusto/Desktop/InfoII/Libreria_LPC845/LPC845/Libreria_LPC845/includes/hpl" -I"/home/augusto/Desktop/InfoII/Libreria_LPC845/LPC845/Puente_levadizo/Modulos" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


