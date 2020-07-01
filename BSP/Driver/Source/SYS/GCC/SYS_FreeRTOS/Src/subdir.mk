################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/wb_aic.c \
F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/wb_cache.c \
F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/wb_config.c \
F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/wb_huart.c \
F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/wb_mmu.c \
F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/wb_power.c \
F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/wb_timer.c \
F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/wb_uart.c 

OBJS += \
./Src/wb_aic.o \
./Src/wb_cache.o \
./Src/wb_config.o \
./Src/wb_huart.o \
./Src/wb_mmu.o \
./Src/wb_power.o \
./Src/wb_timer.o \
./Src/wb_uart.o 

C_DEPS += \
./Src/wb_aic.d \
./Src/wb_cache.d \
./Src/wb_config.d \
./Src/wb_huart.d \
./Src/wb_mmu.d \
./Src/wb_power.d \
./Src/wb_timer.d \
./Src/wb_uart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/wb_aic.o: F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/wb_aic.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=arm926ej-s -marm -mlittle-endian -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -D__FreeRTOS__ -I"F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/GCC/../../../Include" -I"F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/GCC/../../../../ThirdParty/FreeRTOS/Source/include" -I"F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/GCC/../." -I"F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/GCC/../../../../SampleCode/FreeRTOS/ARM9_N9H26" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Src/wb_cache.o: F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/wb_cache.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=arm926ej-s -marm -mlittle-endian -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -D__FreeRTOS__ -I"F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/GCC/../../../Include" -I"F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/GCC/../../../../ThirdParty/FreeRTOS/Source/include" -I"F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/GCC/../." -I"F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/GCC/../../../../SampleCode/FreeRTOS/ARM9_N9H26" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Src/wb_config.o: F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/wb_config.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=arm926ej-s -marm -mlittle-endian -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -D__FreeRTOS__ -I"F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/GCC/../../../Include" -I"F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/GCC/../../../../ThirdParty/FreeRTOS/Source/include" -I"F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/GCC/../." -I"F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/GCC/../../../../SampleCode/FreeRTOS/ARM9_N9H26" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Src/wb_huart.o: F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/wb_huart.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=arm926ej-s -marm -mlittle-endian -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -D__FreeRTOS__ -I"F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/GCC/../../../Include" -I"F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/GCC/../../../../ThirdParty/FreeRTOS/Source/include" -I"F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/GCC/../." -I"F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/GCC/../../../../SampleCode/FreeRTOS/ARM9_N9H26" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Src/wb_mmu.o: F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/wb_mmu.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=arm926ej-s -marm -mlittle-endian -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -D__FreeRTOS__ -I"F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/GCC/../../../Include" -I"F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/GCC/../../../../ThirdParty/FreeRTOS/Source/include" -I"F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/GCC/../." -I"F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/GCC/../../../../SampleCode/FreeRTOS/ARM9_N9H26" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Src/wb_power.o: F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/wb_power.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=arm926ej-s -marm -mlittle-endian -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -D__FreeRTOS__ -I"F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/GCC/../../../Include" -I"F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/GCC/../../../../ThirdParty/FreeRTOS/Source/include" -I"F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/GCC/../." -I"F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/GCC/../../../../SampleCode/FreeRTOS/ARM9_N9H26" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Src/wb_timer.o: F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/wb_timer.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=arm926ej-s -marm -mlittle-endian -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -D__FreeRTOS__ -I"F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/GCC/../../../Include" -I"F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/GCC/../../../../ThirdParty/FreeRTOS/Source/include" -I"F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/GCC/../." -I"F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/GCC/../../../../SampleCode/FreeRTOS/ARM9_N9H26" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Src/wb_uart.o: F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/wb_uart.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=arm926ej-s -marm -mlittle-endian -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -D__FreeRTOS__ -I"F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/GCC/../../../Include" -I"F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/GCC/../../../../ThirdParty/FreeRTOS/Source/include" -I"F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/GCC/../." -I"F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/GCC/../../../../SampleCode/FreeRTOS/ARM9_N9H26" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


