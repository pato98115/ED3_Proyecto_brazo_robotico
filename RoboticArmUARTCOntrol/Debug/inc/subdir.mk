################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../inc/servo_motors.c \
../inc/stepper_motors.c 

OBJS += \
./inc/servo_motors.o \
./inc/stepper_motors.o 

C_DEPS += \
./inc/servo_motors.d \
./inc/stepper_motors.d 


# Each subdirectory must supply rules for building sources it contributes
inc/%.o: ../inc/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSISv2p00_LPC17xx -D__LPC17XX__ -D__REDLIB__ -I"C:\Users\nacho\Documents\LPCXpresso_8.2.2_650\workspace_CMSIS\CMSISv2p00_LPC17xx\Drivers\inc" -I"C:\Users\nacho\Documents\LPCXpresso_8.2.2_650\workspace_CMSIS\RoboticArmUARTControl\inc" -I"C:\Users\nacho\Documents\LPCXpresso_8.2.2_650\workspace_CMSIS\CMSISv2p00_LPC17xx\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


