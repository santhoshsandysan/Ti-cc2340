################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"C:/ti/gcc-arm-none-eabi_7_2_1/bin/arm-none-eabi-gcc-7.2.1.exe" -c -mcpu=cortex-m0plus -march=armv6-m -mthumb -mfloat-abi=soft -I"C:/Users/Yantra/workspace_ccstheia/temperatureNotify_LP_EM_CC2340R5_nortos_gcc" -I"C:/Users/Yantra/workspace_ccstheia/temperatureNotify_LP_EM_CC2340R5_nortos_gcc/Debug" -I"C:/ti/simplelink_lowpower_f3_sdk_9_11_00_18/source" -I"C:/ti/simplelink_lowpower_f3_sdk_9_11_00_18/kernel/nortos" -I"C:/ti/simplelink_lowpower_f3_sdk_9_11_00_18/kernel/nortos/posix" -I"C:/ti/gcc-arm-none-eabi_7_2_1/arm-none-eabi/include/newlib-nano" -I"C:/ti/gcc-arm-none-eabi_7_2_1/arm-none-eabi/include" -O3 -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)" -I"C:/Users/Yantra/workspace_ccstheia/temperatureNotify_LP_EM_CC2340R5_nortos_gcc/Debug/syscfg" -std=c99 $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-297156625: ../temperatureNotify.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"C:/ti/sysconfig_1.23.2/sysconfig_cli.bat" --script "C:/Users/Yantra/workspace_ccstheia/temperatureNotify_LP_EM_CC2340R5_nortos_gcc/temperatureNotify.syscfg" -o "syscfg" -s "C:/ti/simplelink_lowpower_f3_sdk_9_11_00_18/.metadata/product.json" --compiler gcc
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/ti_devices_config.c: build-297156625 ../temperatureNotify.syscfg
syscfg/ti_drivers_config.c: build-297156625
syscfg/ti_drivers_config.h: build-297156625
syscfg/ti_utils_build_linker.cmd.genlibs: build-297156625
syscfg/ti_utils_build_linker.cmd.genmap: build-297156625
syscfg/ti_utils_build_compiler.opt: build-297156625
syscfg/syscfg_c.rov.xs: build-297156625
syscfg: build-297156625

syscfg/%.o: ./syscfg/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"C:/ti/gcc-arm-none-eabi_7_2_1/bin/arm-none-eabi-gcc-7.2.1.exe" -c -mcpu=cortex-m0plus -march=armv6-m -mthumb -mfloat-abi=soft -I"C:/Users/Yantra/workspace_ccstheia/temperatureNotify_LP_EM_CC2340R5_nortos_gcc" -I"C:/Users/Yantra/workspace_ccstheia/temperatureNotify_LP_EM_CC2340R5_nortos_gcc/Debug" -I"C:/ti/simplelink_lowpower_f3_sdk_9_11_00_18/source" -I"C:/ti/simplelink_lowpower_f3_sdk_9_11_00_18/kernel/nortos" -I"C:/ti/simplelink_lowpower_f3_sdk_9_11_00_18/kernel/nortos/posix" -I"C:/ti/gcc-arm-none-eabi_7_2_1/arm-none-eabi/include/newlib-nano" -I"C:/ti/gcc-arm-none-eabi_7_2_1/arm-none-eabi/include" -O3 -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall -MMD -MP -MF"syscfg/$(basename $(<F)).d_raw" -MT"$(@)" -I"C:/Users/Yantra/workspace_ccstheia/temperatureNotify_LP_EM_CC2340R5_nortos_gcc/Debug/syscfg" -std=c99 $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


