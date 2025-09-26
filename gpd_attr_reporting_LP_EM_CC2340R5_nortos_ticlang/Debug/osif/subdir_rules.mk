################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
osif/%.o: ../osif/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -Oz -flto -I"C:/Users/Yantra/workspace_ccstheia/gpd_attr_reporting_LP_EM_CC2340R5_nortos_ticlang" -I"C:/Users/Yantra/workspace_ccstheia/gpd_attr_reporting_LP_EM_CC2340R5_nortos_ticlang/Debug" -I"C:/ti/simplelink_lowpower_f3_sdk_9_11_01_19/source/ti/zigbee/osif/include" -I"C:/ti/simplelink_lowpower_f3_sdk_9_11_01_19/source/ti/zigbee/greenpower/osif/include" -I"C:/ti/simplelink_lowpower_f3_sdk_9_11_01_19/source/third_party/zigbee/greenboss/include" -I"C:/ti/simplelink_lowpower_f3_sdk_9_11_01_19/source/ti/zigbee/include" -I"C:/ti/simplelink_lowpower_f3_sdk_9_11_01_19/source" -I"C:/ti/simplelink_lowpower_f3_sdk_9_11_01_19/kernel/nortos" -I"C:/ti/simplelink_lowpower_f3_sdk_9_11_01_19/kernel/nortos/posix" -DZB_TI_F3_ZGPD -DDEBUG -DNORTOS -gdwarf-3 -MMD -MP -MF"osif/$(basename $(<F)).d_raw" -MT"$(@)" -I"C:/Users/Yantra/workspace_ccstheia/gpd_attr_reporting_LP_EM_CC2340R5_nortos_ticlang/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


