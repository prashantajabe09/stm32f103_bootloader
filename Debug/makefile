################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include Drivers/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := Bootloader_stm32f103x_stm32_protocol
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Bootloader_stm32f103x_stm32_protocol.elf \

MAP_FILES += \
Bootloader_stm32f103x_stm32_protocol.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Bootloader_stm32f103x_stm32_protocol.list \

OBJCOPY_HEX += \
Bootloader_stm32f103x_stm32_protocol.hex \

OBJCOPY_BIN += \
Bootloader_stm32f103x_stm32_protocol.bin \


# All Target
all: main-build

# Main-build Target
main-build: Bootloader_stm32f103x_stm32_protocol.elf secondary-outputs

# Tool invocations
Bootloader_stm32f103x_stm32_protocol.elf Bootloader_stm32f103x_stm32_protocol.map: $(OBJS) $(USER_OBJS) C:\Users\Propix\STM32CubeIDE\workspace_1.9.0\Bootloader_stm32f103x_stm32_protocol\STM32F103C8TX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "Bootloader_stm32f103x_stm32_protocol.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"C:\Users\Propix\STM32CubeIDE\workspace_1.9.0\Bootloader_stm32f103x_stm32_protocol\STM32F103C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Bootloader_stm32f103x_stm32_protocol.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Bootloader_stm32f103x_stm32_protocol.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Bootloader_stm32f103x_stm32_protocol.list"
	@echo 'Finished building: $@'
	@echo ' '

Bootloader_stm32f103x_stm32_protocol.hex: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "Bootloader_stm32f103x_stm32_protocol.hex"
	@echo 'Finished building: $@'
	@echo ' '

Bootloader_stm32f103x_stm32_protocol.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "Bootloader_stm32f103x_stm32_protocol.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) Bootloader_stm32f103x_stm32_protocol.bin Bootloader_stm32f103x_stm32_protocol.elf Bootloader_stm32f103x_stm32_protocol.hex Bootloader_stm32f103x_stm32_protocol.list Bootloader_stm32f103x_stm32_protocol.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
