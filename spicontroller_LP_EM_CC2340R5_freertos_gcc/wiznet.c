#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

/* FreeRTOS / POSIX */
#include <pthread.h>
#include <ti/drivers/Board.h>

/* TI Drivers */
#include <ti/drivers/SPI.h>
#include <ti/display/Display.h>
#include <ti/drivers/GPIO.h>

/* Board-specific defines */
#include "ti_drivers_config.h"

/* SPI handle */
static SPI_Handle spiHandle;
static Display_Handle display;

/* ===== W5500 SPI Command =====
 * Format: [Addr_H][Addr_L][Control][Data...]
 * VERSIONR = 0x0039 (Common register, 1 byte, Read)
 */
#define W5500_VERSIONR_ADDR   0x0039
#define W5500_CTRL_READ       0x00   // Read, 1-byte access, Common reg

/* CS control macros (if not automatic CS) */
#define W5500_CS_ASSERT()   GPIO_write(CONFIG_GPIO_SPI_CONTROLLER_CSN, 0)
#define W5500_CS_DEASSERT() GPIO_write(CONFIG_GPIO_SPI_CONTROLLER_CSN, 1)

/* ===== SPI + W5500 Test ===== */
void w5500_read_version(void)
{
    uint8_t txBuf[4];
    uint8_t rxBuf[4];

    txBuf[0] = (W5500_VERSIONR_ADDR >> 8) & 0xFF; // Addr high
    txBuf[1] = (W5500_VERSIONR_ADDR) & 0xFF;      // Addr low
    txBuf[2] = W5500_CTRL_READ;                   // Control byte
    txBuf[3] = 0x00;                              // Dummy (read)

    SPI_Transaction trans;
    memset(&trans, 0, sizeof(trans));
    trans.count = sizeof(txBuf);
    trans.txBuf = txBuf;
    trans.rxBuf = rxBuf;

    W5500_CS_ASSERT();
    bool ok = SPI_transfer(spiHandle, &trans);
    W5500_CS_DEASSERT();

    if (ok) {
        uint8_t version = rxBuf[3];
        Display_printf(display, 0, 0, "W5500 VERSIONR = 0x%02X", version);
        if (version == 0x04) {
            Display_printf(display, 0, 0, "✅ W5500 detected OK");
        } else {
            Display_printf(display, 0, 0, "⚠ Unexpected version value");
        }
    } else {
        Display_printf(display, 0, 0, "❌ SPI transfer failed");
    }
}

/* ===== FreeRTOS Thread ===== */
void *mainThread2(void *arg0)
{
    /* Open display for UART output */
    Display_Params dp;
    Display_Params_init(&dp);
    display = Display_open(Display_Type_UART, &dp);
    if (display == NULL) {
        while (1);
    }

    Display_printf(display, 0, 0, "Starting W5500 SPI test...");

    /* Init SPI */
    SPI_Params spiParams;
    SPI_Params_init(&spiParams);
    spiParams.bitRate     = 10000000;            // 10 MHz
    spiParams.frameFormat = SPI_POL0_PHA0;      // Mode 0
    // spiParams.dataSize    = 8;

    spiHandle = SPI_open(CONFIG_SPI_CONTROLLER, &spiParams);
if (spiHandle == NULL) {
    Display_printf(display, 0, 0, "SPI_open failed!");
    while (1);
} else {
    Display_printf(display, 0, 0, "SPI_open OK");
}
    /* Init CS pin (manual control) */
    GPIO_setConfig(CONFIG_GPIO_SPI_CONTROLLER_CSN, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);

    /* Small delay for W5500 reset */
    sleep(1);

    /* Try reading version register */
    w5500_read_version();

    Display_printf(display, 0, 0, "Done");

    while (1) {
        // vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskStartScheduler();
}
