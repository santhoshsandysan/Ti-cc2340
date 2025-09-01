################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../cc2340_freertos.cmd 

SYSCFG_SRCS += \
../basic_ble.syscfg 

C_SRCS += \
./syscfg/ti_ble_config.c \
./syscfg/ti_devices_config.c \
./syscfg/ti_radio_config.c \
./syscfg/ti_drivers_config.c \
./syscfg/ti_freertos_config.c \
./syscfg/ti_freertos_portable_config.c \
../main_freertos.c 

GEN_FILES += \
./syscfg/ti_ble_config.c \
./syscfg/ti_devices_config.c \
./syscfg/ti_radio_config.c \
./syscfg/ti_drivers_config.c \
./syscfg/ti_utils_build_compiler.opt \
./syscfg/ti_freertos_config.c \
./syscfg/ti_freertos_portable_config.c 

GEN_MISC_DIRS += \
./syscfg 

C_DEPS += \
./syscfg/ti_ble_config.d \
./syscfg/ti_devices_config.d \
./syscfg/ti_radio_config.d \
./syscfg/ti_drivers_config.d \
./syscfg/ti_freertos_config.d \
./syscfg/ti_freertos_portable_config.d \
./main_freertos.d 

GEN_OPTS += \
./syscfg/ti_utils_build_compiler.opt 

OBJS += \
./syscfg/ti_ble_config.o \
./syscfg/ti_devices_config.o \
./syscfg/ti_radio_config.o \
./syscfg/ti_drivers_config.o \
./syscfg/ti_freertos_config.o \
./syscfg/ti_freertos_portable_config.o \
./main_freertos.o 

GEN_MISC_FILES += \
./syscfg/ti_ble_config.h \
./syscfg/ti_radio_config.h \
./syscfg/ti_drivers_config.h \
./syscfg/ti_utils_build_linker.cmd.genlibs \
./syscfg/ti_utils_build_linker.cmd.genmap \
./syscfg/syscfg_c.rov.xs \
./syscfg/FreeRTOSConfig.h 

GEN_MISC_DIRS__QUOTED += \
"syscfg" 

OBJS__QUOTED += \
"syscfg\ti_ble_config.o" \
"syscfg\ti_devices_config.o" \
"syscfg\ti_radio_config.o" \
"syscfg\ti_drivers_config.o" \
"syscfg\ti_freertos_config.o" \
"syscfg\ti_freertos_portable_config.o" \
"main_freertos.o" 

GEN_MISC_FILES__QUOTED += \
"syscfg\ti_ble_config.h" \
"syscfg\ti_radio_config.h" \
"syscfg\ti_drivers_config.h" \
"syscfg\ti_utils_build_linker.cmd.genlibs" \
"syscfg\ti_utils_build_linker.cmd.genmap" \
"syscfg\syscfg_c.rov.xs" \
"syscfg\FreeRTOSConfig.h" 

C_DEPS__QUOTED += \
"syscfg\ti_ble_config.d" \
"syscfg\ti_devices_config.d" \
"syscfg\ti_radio_config.d" \
"syscfg\ti_drivers_config.d" \
"syscfg\ti_freertos_config.d" \
"syscfg\ti_freertos_portable_config.d" \
"main_freertos.d" 

GEN_FILES__QUOTED += \
"syscfg\ti_ble_config.c" \
"syscfg\ti_devices_config.c" \
"syscfg\ti_radio_config.c" \
"syscfg\ti_drivers_config.c" \
"syscfg\ti_utils_build_compiler.opt" \
"syscfg\ti_freertos_config.c" \
"syscfg\ti_freertos_portable_config.c" 

SYSCFG_SRCS__QUOTED += \
"../basic_ble.syscfg" 

C_SRCS__QUOTED += \
"./syscfg/ti_ble_config.c" \
"./syscfg/ti_devices_config.c" \
"./syscfg/ti_radio_config.c" \
"./syscfg/ti_drivers_config.c" \
"./syscfg/ti_freertos_config.c" \
"./syscfg/ti_freertos_portable_config.c" \
"../main_freertos.c" 


