################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
SYSCFG_SRCS += \
../empty.syscfg 

LDS_SRCS += \
../lpf3_cc23xx_freertos.lds 

C_SRCS += \
../empty.c \
./syscfg/ti_devices_config.c \
./syscfg/ti_drivers_config.c \
./syscfg/ti_freertos_config.c \
./syscfg/ti_freertos_portable_config.c \
../main_freertos.c 

GEN_FILES += \
./syscfg/ti_devices_config.c \
./syscfg/ti_drivers_config.c \
./syscfg/ti_utils_build_compiler.opt \
./syscfg/ti_freertos_config.c \
./syscfg/ti_freertos_portable_config.c 

GEN_MISC_DIRS += \
./syscfg 

C_DEPS += \
./empty.d \
./syscfg/ti_devices_config.d \
./syscfg/ti_drivers_config.d \
./syscfg/ti_freertos_config.d \
./syscfg/ti_freertos_portable_config.d \
./main_freertos.d 

GEN_OPTS += \
./syscfg/ti_utils_build_compiler.opt 

OBJS += \
./empty.o \
./syscfg/ti_devices_config.o \
./syscfg/ti_drivers_config.o \
./syscfg/ti_freertos_config.o \
./syscfg/ti_freertos_portable_config.o \
./main_freertos.o 

GEN_MISC_FILES += \
./syscfg/ti_drivers_config.h \
./syscfg/ti_utils_build_linker.cmd.genlibs \
./syscfg/ti_utils_build_linker.cmd.genmap \
./syscfg/syscfg_c.rov.xs \
./syscfg/FreeRTOSConfig.h 

GEN_MISC_DIRS__QUOTED += \
"syscfg" 

OBJS__QUOTED += \
"empty.o" \
"syscfg\ti_devices_config.o" \
"syscfg\ti_drivers_config.o" \
"syscfg\ti_freertos_config.o" \
"syscfg\ti_freertos_portable_config.o" \
"main_freertos.o" 

GEN_MISC_FILES__QUOTED += \
"syscfg\ti_drivers_config.h" \
"syscfg\ti_utils_build_linker.cmd.genlibs" \
"syscfg\ti_utils_build_linker.cmd.genmap" \
"syscfg\syscfg_c.rov.xs" \
"syscfg\FreeRTOSConfig.h" 

C_DEPS__QUOTED += \
"empty.d" \
"syscfg\ti_devices_config.d" \
"syscfg\ti_drivers_config.d" \
"syscfg\ti_freertos_config.d" \
"syscfg\ti_freertos_portable_config.d" \
"main_freertos.d" 

GEN_FILES__QUOTED += \
"syscfg\ti_devices_config.c" \
"syscfg\ti_drivers_config.c" \
"syscfg\ti_utils_build_compiler.opt" \
"syscfg\ti_freertos_config.c" \
"syscfg\ti_freertos_portable_config.c" 

C_SRCS__QUOTED += \
"../empty.c" \
"./syscfg/ti_devices_config.c" \
"./syscfg/ti_drivers_config.c" \
"./syscfg/ti_freertos_config.c" \
"./syscfg/ti_freertos_portable_config.c" \
"../main_freertos.c" 

SYSCFG_SRCS__QUOTED += \
"../empty.syscfg" 


