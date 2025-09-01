/*
 *  ======== ti_drivers_config.h ========
 *  Configured TI-Drivers module declarations
 *
 *  The macros defines herein are intended for use by applications which
 *  directly include this header. These macros should NOT be hard coded or
 *  copied into library source code.
 *
 *  Symbols declared as const are intended for use with libraries.
 *  Library source code must extern the correct symbol--which is resolved
 *  when the application is linked.
 *
 *  DO NOT EDIT - This file is generated for the LP_EM_CC2340R5
 *  by the SysConfig tool.
 */
#ifndef ti_drivers_config_h
#define ti_drivers_config_h

#define CONFIG_SYSCONFIG_PREVIEW

#define CONFIG_LP_EM_CC2340R5
#ifndef DeviceFamily_CC23X0R5
#define DeviceFamily_CC23X0R5
#endif

#include <ti/devices/DeviceFamily.h>

#include <stdint.h>

/* support C++ sources */
#ifdef __cplusplus
extern "C" {
#endif


/*
 *  ======== Display ========
 */

extern const uint_least8_t                  CONFIG_Display_0_CONST;
#define CONFIG_Display_0                    0
#define CONFIG_TI_DRIVERS_DISPLAY_COUNT     1


/*
 *  ======== GPIO ========
 */
extern const uint_least8_t CONFIG_GPIO_LED_0_CONST;
#define CONFIG_GPIO_LED_0 14

extern const uint_least8_t CONFIG_GPIO_LED_1_CONST;
#define CONFIG_GPIO_LED_1 15

extern const uint_least8_t CONFIG_SPI_CONTROLLER_READY_CONST;
#define CONFIG_SPI_CONTROLLER_READY 10

extern const uint_least8_t CONFIG_SPI_PERIPHERAL_READY_CONST;
#define CONFIG_SPI_PERIPHERAL_READY 8

extern const uint_least8_t SCL_CONST;
#define SCL 24

extern const uint_least8_t SDA_CONST;
#define SDA 0

extern const uint_least8_t ADC1_CONST;
#define ADC1 5

extern const uint_least8_t ADC2_CONST;
#define ADC2 23

extern const uint_least8_t ADC3_CONST;
#define ADC3 1

extern const uint_least8_t CS2_CONST;
#define CS2 25

extern const uint_least8_t CS3_CONST;
#define CS3 2

extern const uint_least8_t CS4_CONST;
#define CS4 7

extern const uint_least8_t CS5_CONST;
#define CS5 21

extern const uint_least8_t CONFIG_GPIO_1_CONST;
#define CONFIG_GPIO_1 9

extern const uint_least8_t CONFIG_GPIO_2_CONST;
#define CONFIG_GPIO_2 19

extern const uint_least8_t CONFIG_GPIO_SPI_CONTROLLER_CSN_CONST;
#define CONFIG_GPIO_SPI_CONTROLLER_CSN 11

/* Owned by CONFIG_SPI_CONTROLLER as  */
extern const uint_least8_t CONFIG_GPIO_SPI_CONTROLLER_SCLK_CONST;
#define CONFIG_GPIO_SPI_CONTROLLER_SCLK 18

/* Owned by CONFIG_SPI_CONTROLLER as  */
extern const uint_least8_t CONFIG_GPIO_SPI_CONTROLLER_POCI_CONST;
#define CONFIG_GPIO_SPI_CONTROLLER_POCI 12

/* Owned by CONFIG_SPI_CONTROLLER as  */
extern const uint_least8_t CONFIG_GPIO_SPI_CONTROLLER_PICO_CONST;
#define CONFIG_GPIO_SPI_CONTROLLER_PICO 13

/* Owned by CONFIG_UART2_0 as  */
extern const uint_least8_t CONFIG_GPIO_UART2_0_TX_CONST;
#define CONFIG_GPIO_UART2_0_TX 20

/* Owned by CONFIG_UART2_0 as  */
extern const uint_least8_t CONFIG_GPIO_UART2_0_RX_CONST;
#define CONFIG_GPIO_UART2_0_RX 22

/* The range of pins available on this device */
extern const uint_least8_t GPIO_pinLowerBound;
extern const uint_least8_t GPIO_pinUpperBound;

/* LEDs are active high */
#define CONFIG_GPIO_LED_ON  (1)
#define CONFIG_GPIO_LED_OFF (0)

#define CONFIG_LED_ON  (CONFIG_GPIO_LED_ON)
#define CONFIG_LED_OFF (CONFIG_GPIO_LED_OFF)


/*
 *  ======== SPI ========
 */

/*
 *  PICO: DIO13
 *  POCI: DIO12
 *  SCLK: DIO18
 *  LaunchPad SPI Bus
 */
extern const uint_least8_t              CONFIG_SPI_CONTROLLER_CONST;
#define CONFIG_SPI_CONTROLLER           0
#define CONFIG_TI_DRIVERS_SPI_COUNT     1


/*
 *  ======== UART2 ========
 */

/*
 *  TX: DIO20A11
 *  RX: DIO22A9
 *  XDS110 UART
 */
extern const uint_least8_t                  CONFIG_UART2_0_CONST;
#define CONFIG_UART2_0                      0
#define CONFIG_TI_DRIVERS_UART2_COUNT       1


/*
 *  ======== Board_init ========
 *  Perform all required TI-Drivers initialization
 *
 *  This function should be called once at a point before any use of
 *  TI-Drivers.
 */
extern void Board_init(void);

/*
 *  ======== Board_initGeneral ========
 *  (deprecated)
 *
 *  Board_initGeneral() is defined purely for backward compatibility.
 *
 *  All new code should use Board_init() to do any required TI-Drivers
 *  initialization _and_ use <Driver>_init() for only where specific drivers
 *  are explicitly referenced by the application.  <Driver>_init() functions
 *  are idempotent.
 */
#define Board_initGeneral Board_init

#ifdef __cplusplus
}
#endif

#endif /* include guard */
