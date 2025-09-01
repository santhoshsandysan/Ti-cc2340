/*
 * Copyright (c) 2018-2024, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== spicontroller.c ========
 */
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* POSIX Header files */
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/SPI.h>
#include <ti/display/Display.h>

/* Driver configuration */
#include "ti_drivers_config.h"

#define THREADSTACKSIZE (1024)

#define SPI_MSG_LENGTH (32)
#define CONTROLLER_MSG ("Hello from controller, msg#: ")

#define MAX_LOOP (10)

#ifdef DeviceFamily_CC35XX
    #define CONFIG_GPIO_LED_0 GPIO_INVALID_INDEX
    #define CONFIG_GPIO_LED_1 GPIO_INVALID_INDEX
#endif

static Display_Handle display;

unsigned char controllerRxBuffer[SPI_MSG_LENGTH];
unsigned char controllerTxBuffer[SPI_MSG_LENGTH];

/* Semaphore to block controller until peripheral is ready for transfer */
sem_t controllerSem;

/*
 *  ======== peripheralReadyFxn ========
 *  Callback function for the GPIO interrupt on CONFIG_SPI_PERIPHERAL_READY.
 */
 static bool w5500_read_reg(SPI_Handle spi, uint16_t addr, uint8_t *buf, uint16_t len);
 static bool max31856_read_reg(SPI_Handle spi, uint8_t reg, uint8_t *buf, uint16_t len);
 static bool max31856_write_reg(SPI_Handle spi, uint8_t reg, uint8_t *buf, uint16_t len);
void peripheralReadyFxn(uint_least8_t index)
{
    sem_post(&controllerSem);
}

/*
 *  ======== controllerThread ========
 *  Controller SPI sends a message to peripheral while simultaneously receiving a
 *  message from the peripheral.
 */
void *controllerThread(void *arg0)
{

            SPI_Handle controllerSpi;
    SPI_Params spiParams;

    while (1) {

    /* Open SPI */
    SPI_Params_init(&spiParams);
    spiParams.dataSize = 8;
    spiParams.frameFormat = SPI_POL0_PHA0;
    spiParams.bitRate = 1000000;
    controllerSpi = SPI_open(CONFIG_SPI_CONTROLLER, &spiParams);
    if (controllerSpi == NULL) {
        Display_printf(display, 0, 0, "Error initializing controller SPI\n");
        while (1) {}
    }
    Display_printf(display, 0, 0, "Controller SPI initialized\n");

        uint8_t buf[6];
    
        /* --- VERSIONR (0x0039) --- */
        if (w5500_read_reg(controllerSpi, 0x0039, buf, 1)) {
            Display_printf(display, 0, 0, "W5500 VERSIONR: 0x%02x", buf[0]);
        }

        /* --- MAC address (0x0009–0x000E) --- */
        if (w5500_read_reg(controllerSpi, 0x0009, buf, 6)) {
            Display_printf(display, 0, 0, "MAC: %02x:%02x:%02x:%02x:%02x:%02x",
                buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);
        }

        /* --- IP address (0x000F–0x0012) --- */
        if (w5500_read_reg(controllerSpi, 0x000F, buf, 4)) {
            Display_printf(display, 0, 0, "IP: %d.%d.%d.%d",
                buf[0], buf[1], buf[2], buf[3]);
        }

        /* --- PHYCFGR (0x002E) --- */
        if (w5500_read_reg(controllerSpi, 0x002E, buf, 1)) {
            Display_printf(display, 0, 0, "PHYCFGR: 0x%02x %s",
                buf[0], (buf[0] & 0x01) ? "LinkUP" : "LinkDOWN");
        }
        SPI_close(controllerSpi);

        sleep(1);

//////////////  Thermo couple //////////////////////////////////////////////////
        uint8_t regVal;
   SPI_Params_init(&spiParams);
    spiParams.dataSize = 8;
    spiParams.frameFormat = SPI_POL0_PHA1;
    spiParams.bitRate = 1000000;
    controllerSpi = SPI_open(CONFIG_SPI_CONTROLLER, &spiParams);
    if (controllerSpi == NULL) {
        Display_printf(display, 0, 0, "Error initializing controller SPI\n");
        while (1) {}
    }
    Display_printf(display, 0, 0, "Controller SPI initialized\n");


// Configure MAX31856 for K-type
uint8_t val;

// CR0 = 0x80 → Continuous conversion, comparator mode, 50Hz filter
val = 0x80;
max31856_write_reg(controllerSpi, 0x00, &val, 1);

// CR1 = 0x03 → K-type thermocouple
val = 0x03;
max31856_write_reg(controllerSpi, 0x01, &val, 1);

Display_printf(display, 0, 0, "MAX31856 configured for K-type");

// Read CR0 (0x00)
if (max31856_read_reg(controllerSpi, 0x00, &regVal, 1)) {
    Display_printf(display, 0, 0, "CR0: 0x%02x", regVal);
}

// Read Fault Status (0x0F)
if (max31856_read_reg(controllerSpi, 0x0A, &regVal, 1)) {
    Display_printf(display, 0, 0, "CJTH: 0x%02x", regVal);
}
if (max31856_read_reg(controllerSpi, 0x0B, &regVal, 1)) {
    Display_printf(display, 0, 0, "CJTL: 0x%02x", regVal);
}
uint8_t tempRaw[3];
if (max31856_read_reg(controllerSpi, 0x0C, tempRaw, 3)) {
    int32_t raw = ((int32_t)tempRaw[0] << 16) | ((int32_t)tempRaw[1] << 8) | tempRaw[2];
    raw >>= 5; // D23–D5 are valid

    float tempC = raw * 0.0078125f; // 1 LSB = 0.0078125 °C
    Display_printf(display, 0, 0, "Thermocouple Temp: %.2f C", tempC);
}


SPI_close(controllerSpi);
sleep(1);
//////////////  Thermo couple //////////////////////////////////////////////////
    }

    SPI_close(controllerSpi);
    Display_printf(display, 0, 0, "\nDone");
    return NULL;
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


static bool w5500_read_reg(SPI_Handle spi, uint16_t addr, uint8_t *buf, uint16_t len)
{
    SPI_Transaction trans;
    uint8_t txBuf[3 + 8] = {0};  // header + max 8 bytes (adjust if needed)
    uint8_t rxBuf[3 + 8] = {0};

    if (len > 8) return false;   // simple limit

    txBuf[0] = (addr >> 8) & 0xFF;   // Address high
    txBuf[1] = addr & 0xFF;          // Address low
    txBuf[2] = 0x00;                 // Control byte (common reg read)
    memset(&trans, 0, sizeof(trans));
    trans.count = 3 + len;
    trans.txBuf = txBuf;
    trans.rxBuf = rxBuf;

    /* Assert CS */
    GPIO_write(CONFIG_GPIO_SPI_CONTROLLER_CSN, 0);

    bool ok = SPI_transfer(spi, &trans);

    /* Deassert CS */
    GPIO_write(CONFIG_GPIO_SPI_CONTROLLER_CSN, 1);

    if (ok) {
        for (int i = 0; i < len; i++) {
            buf[i] = rxBuf[3 + i];
        }
    }
    return ok;
}

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    pthread_t thread0;
    pthread_attr_t attrs;
    struct sched_param priParam;
    int retc;
    int detachState;

    /* Call driver init functions. */
    Display_init();
    GPIO_init();
    SPI_init();

    /* Configure the LED pins */
    GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_LED_1, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    /* Open the display for output */
    display = Display_open(Display_Type_UART, NULL);
    if (display == NULL)
    {
        /* Failed to open display driver */
        while (1) {}
    }

    /* Turn on user LED */
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);

    Display_printf(display, 0, 0, "Starting the SPI controller example");
    Display_printf(display,
                   0,
                   0,
                   "This example requires external wires to be "
                   "connected to the header pins. Please see the Readme.html and Board.html for details.\n");

    /* Create application threads */
    pthread_attr_init(&attrs);

    detachState = PTHREAD_CREATE_DETACHED;
    /* Set priority and stack size attributes */
    retc        = pthread_attr_setdetachstate(&attrs, detachState);
    if (retc != 0)
    {
        /* pthread_attr_setdetachstate() failed */
        while (1) {}
    }

    retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
    if (retc != 0)
    {
        /* pthread_attr_setstacksize() failed */
        while (1) {}
    }

    /* Create controller thread */
    priParam.sched_priority = 1;
    pthread_attr_setschedparam(&attrs, &priParam);

    retc = pthread_create(&thread0, &attrs, controllerThread, NULL);
    if (retc != 0)
    {
        /* pthread_create() failed */
        while (1) {}
    }

    return (NULL);
}
