################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS

GEN_OPTS__FLAG := @"syscfg/ti_utils_build_compiler.opt" 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./on_off_light.o" \
"./syscfg/ti_devices_config.o" \
"./syscfg/ti_radio_config.o" \
"./syscfg/ti_drivers_config.o" \
"./syscfg/ti_zigbee_config.o" \
"./syscfg/ti_freertos_config.o" \
"./syscfg/ti_freertos_portable_config.o" \
"./osif/ti_f3_led_buttons.o" \
"./osif/ti_f3_main.o" \
"../lpf3_zigbee_freertos.cmd" \
$(GEN_CMDS__FLAG) \
-Wl,-lti_utils_build_linker.cmd.genlibs \
-Wl,-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include osif/subdir_vars.mk
-include subdir_rules.mk
-include osif/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
onoff_light_LP_EM_CC2340R5_freertos_ticlang.out 

EXE_OUTPUTS__QUOTED += \
"onoff_light_LP_EM_CC2340R5_freertos_ticlang.out" 


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "onoff_light_LP_EM_CC2340R5_freertos_ticlang.out"

# Tool invocations
onoff_light_LP_EM_CC2340R5_freertos_ticlang.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: Arm Linker'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -Oz -flto -DSYNC_WRITE -DZB_TI_F3_ZBOSS -gdwarf-3 -Wno-gnu-folding-constant -Wno-unaligned-access -Wl,-m"onoff_light_LP_EM_CC2340R5_freertos_ticlang.map" -Wl,-i"C:/ti/simplelink_lowpower_f3_sdk_9_11_01_19/source" -Wl,-i"C:/Users/Yantra/workspace_ccstheia/onoff_light_LP_EM_CC2340R5_freertos_ticlang/Debug/syscfg" -Wl,-i"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/lib" -Wl,--reread_libs -Wl,--diag_wrap=off -Wl,--display_error_number -Wl,--warn_sections -Wl,--xml_link_info="onoff_light_LP_EM_CC2340R5_freertos_ticlang_linkInfo.xml" -Wl,--rom_model -o "onoff_light_LP_EM_CC2340R5_freertos_ticlang.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(GEN_MISC_FILES__QUOTED)$(GEN_FILES__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "on_off_light.o" "syscfg\ti_devices_config.o" "syscfg\ti_radio_config.o" "syscfg\ti_drivers_config.o" "syscfg\ti_zigbee_config.o" "syscfg\ti_freertos_config.o" "syscfg\ti_freertos_portable_config.o" "osif\ti_f3_led_buttons.o" "osif\ti_f3_main.o" 
	-$(RM) "on_off_light.d" "syscfg\ti_devices_config.d" "syscfg\ti_radio_config.d" "syscfg\ti_drivers_config.d" "syscfg\ti_zigbee_config.d" "syscfg\ti_freertos_config.d" "syscfg\ti_freertos_portable_config.d" "osif\ti_f3_led_buttons.d" "osif\ti_f3_main.d" 
	-$(RMDIR) $(GEN_MISC_DIRS__QUOTED)
	-@echo 'Finished clean'
	-@echo ' '

post-build:
	-C:/ti/simplelink_lowpower_f3_sdk_9_11_01_19/tools/common/crc_tool/crc_tool patch-image --elf C:/Users/Yantra/workspace_ccstheia/onoff_light_LP_EM_CC2340R5_freertos_ticlang/Debug/onoff_light_LP_EM_CC2340R5_freertos_ticlang.out --symbol-prefix ti_utils_build_GenMap_sym_CRC --output C:/Users/Yantra/workspace_ccstheia/onoff_light_LP_EM_CC2340R5_freertos_ticlang/Debug/onoff_light_LP_EM_CC2340R5_freertos_ticlang.out
	-C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmobjcopy -O ihex C:/Users/Yantra/workspace_ccstheia/onoff_light_LP_EM_CC2340R5_freertos_ticlang/Debug/onoff_light_LP_EM_CC2340R5_freertos_ticlang.out C:/Users/Yantra/workspace_ccstheia/onoff_light_LP_EM_CC2340R5_freertos_ticlang/Debug/onoff_light_LP_EM_CC2340R5_freertos_ticlang.hex
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

