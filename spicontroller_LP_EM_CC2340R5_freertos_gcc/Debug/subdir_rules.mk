################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"C:/ti/gcc-arm-none-eabi_7_2_1/bin/arm-none-eabi-gcc-7.2.1.exe" -c -mcpu=cortex-m0plus -march=armv6-m -mthumb -mfloat-abi=soft -D_REENT_SMALL -I"C:/Users/Yantra/workspace_ccstheia/spicontroller_LP_EM_CC2340R5_freertos_gcc" -I"C:/Users/Yantra/workspace_ccstheia/spicontroller_LP_EM_CC2340R5_freertos_gcc/Debug" -I"C:/ti/simplelink_lowpower_f3_sdk_9_11_00_18/source" -I"C:/ti/simplelink_lowpower_f3_sdk_9_11_00_18/kernel/freertos" -I"C:/ti/simplelink_lowpower_f3_sdk_9_11_00_18/source/ti/posix/gcc" -I"C:/ti/simplelink_lowpower_f3_sdk_9_11_00_18/source/third_party/freertos/include" -I"C:/ti/simplelink_lowpower_f3_sdk_9_11_00_18/source/third_party/freertos/portable/GCC/ARM_CM0" -I"C:/ti/gcc-arm-none-eabi_7_2_1/arm-none-eabi/include/newlib-nano" -I"C:/ti/gcc-arm-none-eabi_7_2_1/arm-none-eabi/include" -O3 -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)" -I"C:/Users/Yantra/workspace_ccstheia/spicontroller_LP_EM_CC2340R5_freertos_gcc/Debug/syscfg" -std=c99 $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-853341631: ../spicontroller.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"C:/ti/sysconfig_1.23.2/sysconfig_cli.bat" --script "C:/Users/Yantra/workspace_ccstheia/spicontroller_LP_EM_CC2340R5_freertos_gcc/spicontroller.syscfg" -o "syscfg" -s "C:/ti/simplelink_lowpower_f3_sdk_9_11_00_18/.metadata/product.json" --compiler gcc
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/ti_devices_config.c: build-853341631 ../spicontroller.syscfg
syscfg/ti_drivers_config.c: build-853341631
syscfg/ti_drivers_config.h: build-853341631
syscfg/ti_utils_build_linker.cmd.genlibs: build-853341631
syscfg/ti_utils_build_linker.cmd.genmap: build-853341631
syscfg/ti_utils_build_compiler.opt: build-853341631
syscfg/syscfg_c.rov.xs: build-853341631
syscfg/FreeRTOSConfig.h: build-853341631
syscfg/ti_freertos_config.c: build-853341631
syscfg/ti_freertos_portable_config.c: build-853341631
syscfg: build-853341631

syscfg/%.o: ./syscfg/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"C:/ti/gcc-arm-none-eabi_7_2_1/bin/arm-none-eabi-gcc-7.2.1.exe" -c -mcpu=cortex-m0plus -march=armv6-m -mthumb -mfloat-abi=soft -D_REENT_SMALL -I"C:/Users/Yantra/workspace_ccstheia/spicontroller_LP_EM_CC2340R5_freertos_gcc" -I"C:/Users/Yantra/workspace_ccstheia/spicontroller_LP_EM_CC2340R5_freertos_gcc/Debug" -I"C:/ti/simplelink_lowpower_f3_sdk_9_11_00_18/source" -I"C:/ti/simplelink_lowpower_f3_sdk_9_11_00_18/kernel/freertos" -I"C:/ti/simplelink_lowpower_f3_sdk_9_11_00_18/source/ti/posix/gcc" -I"C:/ti/simplelink_lowpower_f3_sdk_9_11_00_18/source/third_party/freertos/include" -I"C:/ti/simplelink_lowpower_f3_sdk_9_11_00_18/source/third_party/freertos/portable/GCC/ARM_CM0" -I"C:/ti/gcc-arm-none-eabi_7_2_1/arm-none-eabi/include/newlib-nano" -I"C:/ti/gcc-arm-none-eabi_7_2_1/arm-none-eabi/include" -O3 -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall -MMD -MP -MF"syscfg/$(basename $(<F)).d_raw" -MT"$(@)" -I"C:/Users/Yantra/workspace_ccstheia/spicontroller_LP_EM_CC2340R5_freertos_gcc/Debug/syscfg" -std=c99 $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


