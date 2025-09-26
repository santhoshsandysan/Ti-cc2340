
#include <stdio.h>
#include <FreeRTOS.h>
#include <stdint.h>
#include <task.h>
#ifdef __ICCARM__
    #include <DLib_Threads.h>
#endif
#include <ti/drivers/Power.h>
#include <ti/devices/DeviceFamily.h>

#include "ti/ble/stack_util/icall/app/icall.h"
#include "ti/ble/stack_util/health_toolkit/assert.h"
#include "ti/ble/stack_util/bcomdef.h"

#ifndef USE_DEFAULT_USER_CFG
#include "ti/ble/app_util/config/ble_user_config.h"
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Board.h>
#include <ti/display/Display.h>

// Temperature sensor 
#include <ti/drivers/Temperature.h>
#include "ti_drivers_config.h"
#include <ti/ble/profiles/simple_gatt/simple_gatt_profile.h>

#include <ti/drivers/GPIO.h>
#include <ti/drivers/SPI.h>

#include <ti/drivers/batterymonitor/BatMonSupportLPF3.h>    

#include <ti/drivers/BatteryMonitor.h>


int16_t currentTemperature;

// BLE user defined configuration
icall_userCfg_t user0Cfg = BLE_USER_CFG;
#endif // USE_DEFAULT_USER_CFG

static Display_Handle display;


extern void appMain(void);
extern void AssertHandler(uint8 assertCause, uint8 assertSubcause);

static bool max31856_read_reg(SPI_Handle spi, uint8_t reg, uint8_t *buf, uint16_t len);
static bool max31856_write_reg(SPI_Handle spi, uint8_t reg, uint8_t *buf, uint16_t len);


static void readBatteryVoltageUTF8(char *buffer, size_t bufferSize)
{
    uint16_t milliVolts;

    // Initialize driver (only once in your system)
    BatteryMonitor_init();

    // Get current voltage in millivolts
    milliVolts = BatteryMonitor_getVoltage();

    float volts = milliVolts / 1000.0f;

    // Format string in UTF-8 with 2 decimal places
    snprintf(buffer, bufferSize, "🔋 %.2f V", volts);
}

int main()
{
  /* Register Application callback to trap asserts raised in the Stack */
  halAssertCback = AssertHandler;
  RegisterAssertCback(AssertHandler);

  Board_init();
  GPIO_init();
    SPI_init();


      display = Display_open(Display_Type_UART, NULL);
    if (display == NULL)
    {
        /* Failed to open display driver */
        while (1) {}
    }


  /* Update User Configuration of the stack */
  user0Cfg.appServiceInfo->timerTickPeriod = ICall_getTickPeriod();
  user0Cfg.appServiceInfo->timerMaxMillisecond  = ICall_getMaxMSecs();
  printf("Hello world");

  /* Initialize all applications tasks */
  appMain();


  /* Start the FreeRTOS scheduler */
  vTaskStartScheduler();

  return 0;

}

void ledTask500ms(void *pvParameters)
{
    while (1)
    {
        GPIO_toggle(CONFIG_GPIO_LED_GREEN);
        vTaskDelay(pdMS_TO_TICKS(500)); // Delay 500 ms
    }
}

void ledTask1000ms(void *pvParameters)
{
    while (1)
    {
        // SimpleGatt_notifyChar4();
        GPIO_toggle(CONFIG_GPIO_LED_RED);
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay 1000 ms
    }
}

void temptask(void *pvParameters)
{
    Temperature_init();
    SPI_Handle controllerSpi;
    SPI_Params spiParams;

    // Open SPI once
    SPI_Params_init(&spiParams);
    spiParams.dataSize = 8;
    spiParams.frameFormat = SPI_POL0_PHA1;
    spiParams.bitRate = 1000000;

    controllerSpi = SPI_open(CONFIG_SPI_CONTROLLER, &spiParams);
    if (controllerSpi == NULL) {
        vTaskDelete(NULL); // kill this task
    }

    

    // Configure MAX31856 for K-type (do it once)

    while (1)
    {
            uint8_t val;

    // CR0 = 0x80 → Continuous conversion, comparator mode, 50Hz filter
    val = 0x80;
    max31856_write_reg(controllerSpi, 0x00, &val, 1);

    // CR1 = 0x03 → K-type thermocouple
    val = 0x03;
    max31856_write_reg(controllerSpi, 0x01, &val, 1);


        uint8_t regVal;
        float tempC;
        uint8_t tempRaw[3];

        // Read CR0 just for debugging
        if (max31856_read_reg(controllerSpi, 0x00, &regVal, 1)) {

        }

        // Read CJT (cold junction) registers
        if (max31856_read_reg(controllerSpi, 0x0A, &regVal, 1)) {

        }
        if (max31856_read_reg(controllerSpi, 0x0B, &regVal, 1)) {

        }

        // Read thermocouple temperature (3 bytes, 24-bit signed)
        if (max31856_read_reg(controllerSpi, 0x0C, tempRaw, 3)) {
            int32_t raw = ((int32_t)tempRaw[0] << 16) |
                          ((int32_t)tempRaw[1] << 8) |
                          tempRaw[2];
            raw >>= 5; // D23–D5 are valid

            tempC = raw * 0.0078125f; // 1 LSB = 0.0078125 °C
            float temperature = tempC;

uint8_t charValue5[SIMPLEGATTPROFILE_CHAR5_LEN];

// format into string (UTF-8 = ASCII here)
snprintf((char *)charValue5, SIMPLEGATTPROFILE_CHAR5_LEN, "%.2f", temperature);

// Update GATT characteristic

// char voltageStr[64];  // UTF-8 string buffer
// readBatteryVoltageUTF8(voltageStr, sizeof(voltageStr));
// printf("%s\n", voltageStr);  // prints with UTF-8 battery emoji


// SimpleGattProfile_setParameter(SIMPLEGATTPROFILE_CHAR5,
//                                SIMPLEGATTPROFILE_CHAR5_LEN,
//                                voltageStr);
SimpleGattProfile_setParameter(SIMPLEGATTPROFILE_CHAR5,
                               SIMPLEGATTPROFILE_CHAR5_LEN,
                               charValue5);

    Temperature_init();

    float currentTemperature = Temperature_getTemperature();

    uint8_t charValue6[SIMPLEGATTPROFILE_CHAR6_LEN];

    // format into string (UTF-8 = ASCII here)
    snprintf((char *)charValue6, SIMPLEGATTPROFILE_CHAR6_LEN, "%.2f", currentTemperature);

    // Update GATT characteristic
    SimpleGattProfile_setParameter(SIMPLEGATTPROFILE_CHAR6,
                                SIMPLEGATTPROFILE_CHAR6_LEN,
                                charValue6);



            // Scale and send over BLE
            uint32_t scaledTemp = (uint32_t)(tempC * 100); // e.g. 450.22 → 45022

            // If you only want 2 chars (max 655.35 °C):
            uint8_t lowByte  = scaledTemp & 0xFF;
            uint8_t highByte = (scaledTemp >> 8) & 0xFF;
            SimpleGattProfile_setParameter(SIMPLEGATTPROFILE_CHAR1, sizeof(uint8_t), &lowByte);
            SimpleGattProfile_setParameter(SIMPLEGATTPROFILE_CHAR2, sizeof(uint8_t), &highByte);

            // If you want 3 chars (up to 167772.15 °C), uncomment:
            /*
            uint8_t byte1 = scaledTemp & 0xFF;
            uint8_t byte2 = (scaledTemp >> 8) & 0xFF;
            uint8_t byte3 = (scaledTemp >> 16) & 0xFF;
            SimpleGattProfile_setParameter(SIMPLEGATTPROFILE_CHAR1, sizeof(uint8_t), &byte1);
            SimpleGattProfile_setParameter(SIMPLEGATTPROFILE_CHAR2, sizeof(uint8_t), &byte2);
            SimpleGattProfile_setParameter(SIMPLEGATTPROFILE_CHAR3, sizeof(uint8_t), &byte3);
            */



        }

        // GPIO_toggle(CONFIG_GPIO_LED_RED);

        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay 1s
    }

    // Never reached, but safe cleanup
    SPI_close(controllerSpi);
}



static bool max31856_write_reg(SPI_Handle spi, uint8_t reg, uint8_t *buf, uint16_t len)
{
    SPI_Transaction trans;
    uint8_t txBuf[1 + 8] = {0};

    if (len > 8) return false;

    // MSB=1 → write
    txBuf[0] = reg | 0x80;
    for (int i = 0; i < len; i++) {
        txBuf[1 + i] = buf[i];
    }

    memset(&trans, 0, sizeof(trans));
    trans.count = 1 + len;
    trans.txBuf = txBuf;
    trans.rxBuf = NULL;

    GPIO_write(CS2, 0);
    bool ok = SPI_transfer(spi, &trans);
    GPIO_write(CS2, 1);

    return ok;
}


static bool max31856_read_reg(SPI_Handle spi, uint8_t reg, uint8_t *buf, uint16_t len)
{
    SPI_Transaction trans;
    uint8_t txBuf[1 + 8] = {0};   // addr + up to 8 bytes
    uint8_t rxBuf[1 + 8] = {0};

    if (len > 8) return false;    // safety limit

    // First byte = register address (MSB=0 → read)
    txBuf[0] = reg & 0x7F;

    memset(&trans, 0, sizeof(trans));
    trans.count = 1 + len;        // 1 addr + len data
    trans.txBuf = txBuf;
    trans.rxBuf = rxBuf;

    // Assert CS
    GPIO_write(CS2, 0);

    bool ok = SPI_transfer(spi, &trans);

    // Deassert CS
    GPIO_write(CS2, 1);

    if (ok) {
        for (int i = 0; i < len; i++) {
            buf[i] = rxBuf[1 + i];   // skip address byte
        }
    }
    return ok;
}

//*****************************************************************************
//
//! \brief Application defined stack overflow hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    //Handle FreeRTOS Stack Overflow
    AssertHandler(HAL_ASSERT_CAUSE_STACK_OVERFLOW_ERROR, 0);
}


void AssertHandler(uint8_t assertCause, uint8_t assertSubcause)
{
    // check the assert cause
    switch(assertCause)
    {
        case HAL_ASSERT_CAUSE_OUT_OF_MEMORY:
        {
            // ERROR: OUT OF MEMORY
            HAL_ASSERT_SPINLOCK;
            break;
        }

        case HAL_ASSERT_CAUSE_INTERNAL_ERROR:
        {
            // check the subcause
            if(assertSubcause == HAL_ASSERT_SUBCAUSE_FW_INERNAL_ERROR)
            {
                // ERROR: INTERNAL FW ERROR
                HAL_ASSERT_SPINLOCK;
            }
            else
            {
                // ERROR: INTERNAL ERROR
                HAL_ASSERT_SPINLOCK;
            }
            break;
        }

        case HAL_ASSERT_CAUSE_ICALL_ABORT:
        {
            HAL_ASSERT_SPINLOCK;
            break;
        }

        case HAL_ASSERT_CAUSE_ICALL_TIMEOUT:
        {
            HAL_ASSERT_SPINLOCK;
            break;
        }

        case HAL_ASSERT_CAUSE_WRONG_API_CALL:
        {
            HAL_ASSERT_SPINLOCK;
            break;
        }

        case HAL_ASSERT_CAUSE_STACK_OVERFLOW_ERROR:
        {
            HAL_ASSERT_SPINLOCK;
            break;
        }

        case HAL_ASSERT_CAUSE_LL_INIT_RNG_NOISE_FAILURE:
        {
            HAL_ASSERT_SPINLOCK;
            break;
        }

        default:
        {
            HAL_ASSERT_SPINLOCK;
            break;
        }
    }

    return;
}

