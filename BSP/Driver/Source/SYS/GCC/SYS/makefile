################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: libN9H26_SYS.a secondary-outputs

# Tool invocations
libN9H26_SYS.a: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM Cross Archiver'
	arm-none-eabi-ar -r  "libN9H26_SYS.a" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(ARCHIVES)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS) libN9H26_SYS.a
	-@echo ' '

post-build:
	-cp -p "F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/GCC/SYS/libN9H26_SYS.a" "F:/ARM_Project/W55FA92/N9H20/BSP/Driver/Source/SYS/GCC/../../../../Library_GCC/IPLib/libN9H26_SYS.a"
	-@echo ' '

secondary-outputs:

.PHONY: all clean dependents post-build

-include ../makefile.targets
