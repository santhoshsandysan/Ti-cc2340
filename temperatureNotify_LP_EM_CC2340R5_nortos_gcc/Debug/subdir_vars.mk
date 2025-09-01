################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
SYSCFG_SRCS += \
../temperatureNotify.syscfg 

LDS_SRCS += \
../lpf3_cc23xx_nortos.lds 

C_SRCS += \
../main_nortos.c \
../temperatureNotify.c \
./syscfg/ti_devices_config.c \
./syscfg/ti_drivers_config.c 

GEN_FILES += \
./syscfg/ti_devices_config.c \
./syscfg/ti_drivers_config.c \
./syscfg/ti_utils_build_compiler.opt 

GEN_MISC_DIRS += \
./syscfg 

C_DEPS += \
./main_nortos.d \
./temperatureNotify.d \
./syscfg/ti_devices_config.d \
./syscfg/ti_drivers_config.d 

GEN_OPTS += \
./syscfg/ti_utils_build_compiler.opt 

OBJS += \
./main_nortos.o \
./temperatureNotify.o \
./syscfg/ti_devices_config.o \
./syscfg/ti_drivers_config.o 

GEN_MISC_FILES += \
./syscfg/ti_drivers_config.h \
./syscfg/ti_utils_build_linker.cmd.genlibs \
./syscfg/ti_utils_build_linker.cmd.genmap \
./syscfg/syscfg_c.rov.xs 

GEN_MISC_DIRS__QUOTED += \
"syscfg" 

OBJS__QUOTED += \
"main_nortos.o" \
"temperatureNotify.o" \
"syscfg\ti_devices_config.o" \
"syscfg\ti_drivers_config.o" 

GEN_MISC_FILES__QUOTED += \
"syscfg\ti_drivers_config.h" \
"syscfg\ti_utils_build_linker.cmd.genlibs" \
"syscfg\ti_utils_build_linker.cmd.genmap" \
"syscfg\syscfg_c.rov.xs" 

C_DEPS__QUOTED += \
"main_nortos.d" \
"temperatureNotify.d" \
"syscfg\ti_devices_config.d" \
"syscfg\ti_drivers_config.d" 

GEN_FILES__QUOTED += \
"syscfg\ti_devices_config.c" \
"syscfg\ti_drivers_config.c" \
"syscfg\ti_utils_build_compiler.opt" 

C_SRCS__QUOTED += \
"../main_nortos.c" \
"../temperatureNotify.c" \
"./syscfg/ti_devices_config.c" \
"./syscfg/ti_drivers_config.c" 

SYSCFG_SRCS__QUOTED += \
"../temperatureNotify.syscfg" 


