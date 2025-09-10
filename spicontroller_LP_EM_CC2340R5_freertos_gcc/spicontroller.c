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

 static int count = 0; // keep track of messages


 static bool w5500_read_reg(SPI_Handle spi, uint16_t addr, uint8_t *buf, uint16_t len);
 static bool w5500_write_reg(SPI_Handle spi, uint16_t addr, const uint8_t *buf, uint16_t len);
 static bool max31856_read_reg(SPI_Handle spi, uint8_t reg, uint8_t *buf, uint16_t len);
 static bool max31856_write_reg(SPI_Handle spi, uint8_t reg, uint8_t *buf, uint16_t len);
 void sendHelloDirect(SPI_Handle spi)
{
uint8_t val;
uint16_t txptr;
uint8_t txwr[2];

// ---------- Configure Socket 0 (TCP) ----------


// Set TCP mode
val = 0x01;
w5500_write_reg(spi, 0x4000, &val, 1);  // Sn_MR

// Set source port 5000
uint8_t sport[2] = {0x13, 0x88};
w5500_write_reg(spi, 0x4004, sport, 2);

// Open socket
val = 0x01;
w5500_write_reg(spi, 0x4001, &val, 1);
do { w5500_read_reg(spi, 0x4001, &val, 1); } while (val);

// Set destination IP & port (PC)
uint8_t dstIP[4] = {192, 168, 1, 100};   // PC IP
uint8_t dstPort[2] = {0x17, 0x70};       // 6000
w5500_write_reg(spi, 0x400C, dstIP, 4);  // Sn_DIPR
w5500_write_reg(spi, 0x4010, dstPort, 2);// Sn_DPORT

// Connect
val = 0x04;  // CONNECT
w5500_write_reg(spi, 0x4001, &val, 1);
do { w5500_read_reg(spi, 0x4001, &val, 1); } while (val);

// Wait for ESTABLISHED
do {
    w5500_read_reg(spi, 0x4003, &val, 1); // Sn_SR
} while (val != 0x17);

// ---------- Send clean ASCII message ----------
const char msg[] = "Hello from CC2340!\r\n";
uint16_t tx_ptr;
uint8_t tx_buf[2];

char msg_buf[64];  // buffer to hold "Hello from CC2340! #n"
uint16_t len = sizeof(msg_buf)-1;
snprintf(msg_buf, sizeof(msg_buf), "Hello from CC2340! #%d\r\n", count++);
Display_printf(display, 0, 0, "%s", msg_buf);


// Read TX write pointer
w5500_read_reg(spi, 0x4024, tx_buf, 2);
tx_ptr = (tx_buf[0] << 8) | tx_buf[1];

// Calculate buffer address
uint16_t offset = tx_ptr & 0x07FF; // 2KB buffer
uint16_t addr = 0x8000 + offset;

// Write message bytes
for (uint16_t i = 0; i < len; i++) {
    w5500_write_reg(spi, addr + i, (uint8_t*)&msg_buf[i], 1);
}

// Update TX write pointer
tx_ptr += len;
tx_buf[0] = (tx_ptr >> 8) & 0xFF;
tx_buf[1] = tx_ptr & 0xFF;
w5500_write_reg(spi, 0x4024, tx_buf, 2);

// Send
uint8_t cmd = 0x20;
w5500_write_reg(spi, 0x4001, &cmd, 1);
do { w5500_read_reg(spi, 0x4001, &cmd, 1); } while (cmd != 0);


}

static uint8_t w5500_get_ctrl(uint16_t addr, bool isWrite)
{
    uint8_t block;

    if (addr < 0x4000) {
        // Common registers
        block = 0x00;
    } else if (addr < 0x4800) {
        // Socket 0 registers
        block = 0x08;
    } else if (addr >= 0x8000 && addr < 0xA000) {
        // Socket 0 TX buffer
        block = 0x10;
    } else if (addr >= 0xC000 && addr < 0xE000) {
        // Socket 0 RX buffer
        block = 0x18;
    } else {
        block = 0x00; // fallback
    }

    return block | (isWrite ? 0x04 : 0x00);
}

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
        // uint8_t reset = 0x80;
        // w5500_write_reg(controllerSpi, 0x0000, &reset, 1);  // MR
        
        SPI_Handle spi = controllerSpi;
        

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

        uint8_t status;
w5500_read_reg(controllerSpi, 0x4003, &status, 1); // Sn_SR
Display_printf(display, 0, 0, "Sn_SR: 0x%02x", status);

uint8_t mac[6] = {0x00,0x08,0xDC,0x11,0x22,0x33}; // unique MAC
uint8_t ip[4]  = {192,168,1,50};  // W5500 IP
uint8_t sub[4] = {255,255,255,0}; // Subnet mask
uint8_t gw[4]  = {192,168,1,1};   // Gateway (can be 192.168.1.1 or 0.0.0.0 if direct PC connection)

w5500_write_reg(spi, 0x0001, gw, 4);   // GAR
w5500_write_reg(spi, 0x0005, sub, 4);  // SUBR
w5500_write_reg(spi, 0x0009, mac, 6);  // SHAR
w5500_write_reg(spi, 0x000F, ip, 4);   // SIPR



sleep(1);
sendHelloDirect(controllerSpi);





        SPI_close(controllerSpi);

        sleep(1);
        

//////////////  Thermo couple //////////////////////////////////////////////////
//         uint8_t regVal;
//    SPI_Params_init(&spiParams);
//     spiParams.dataSize = 8;
//     spiParams.frameFormat = SPI_POL0_PHA1;
//     spiParams.bitRate = 1000000;
//     controllerSpi = SPI_open(CONFIG_SPI_CONTROLLER, &spiParams);
//     if (controllerSpi == NULL) {
//         Display_printf(display, 0, 0, "Error initializing controller SPI\n");
//         while (1) {}
//     }
//     Display_printf(display, 0, 0, "Controller SPI initialized\n");


// // Configure MAX31856 for K-type
// uint8_t val;

// // CR0 = 0x80 → Continuous conversion, comparator mode, 50Hz filter
// val = 0x80;
// max31856_write_reg(controllerSpi, 0x00, &val, 1);

// // CR1 = 0x03 → K-type thermocouple
// val = 0x03;
// max31856_write_reg(controllerSpi, 0x01, &val, 1);

// Display_printf(display, 0, 0, "MAX31856 configured for K-type");

// // Read CR0 (0x00)
// if (max31856_read_reg(controllerSpi, 0x00, &regVal, 1)) {
//     Display_printf(display, 0, 0, "CR0: 0x%02x", regVal);
// }

// // Read Fault Status (0x0F)
// if (max31856_read_reg(controllerSpi, 0x0A, &regVal, 1)) {
//     Display_printf(display, 0, 0, "CJTH: 0x%02x", regVal);
// }
// if (max31856_read_reg(controllerSpi, 0x0B, &regVal, 1)) {
//     Display_printf(display, 0, 0, "CJTL: 0x%02x", regVal);
// }
// uint8_t tempRaw[3];
// if (max31856_read_reg(controllerSpi, 0x0C, tempRaw, 3)) {
//     int32_t raw = ((int32_t)tempRaw[0] << 16) | ((int32_t)tempRaw[1] << 8) | tempRaw[2];
//     raw >>= 5; // D23–D5 are valid

        //     float tempC = raw * 0.0078125f; // 1 LSB = 0.0078125 °C
        //     Display_printf(display, 0, 0, "Thermocouple Temp: %.2f C",
        //     tempC);
        // }

        // SPI_close(controllerSpi);
        // sleep(1);
        // //////////////  Thermo couple
        // //////////////////////////////////////////////////
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
    uint8_t txBuf[3 + 8] = {0};
    uint8_t rxBuf[3 + 8] = {0};

    if (len > 8) return false;

    txBuf[0] = (addr >> 8) & 0xFF;
    txBuf[1] = addr & 0xFF;
    txBuf[2] = w5500_get_ctrl(addr, false);  // <-- FIX HERE

    memset(&trans, 0, sizeof(trans));
    trans.count = 3 + len;
    trans.txBuf = txBuf;
    trans.rxBuf = rxBuf;

    GPIO_write(CONFIG_GPIO_SPI_CONTROLLER_CSN, 0);
    bool ok = SPI_transfer(spi, &trans);
    GPIO_write(CONFIG_GPIO_SPI_CONTROLLER_CSN, 1);

    if (ok) {
        for (int i = 0; i < len; i++) {
            buf[i] = rxBuf[3 + i];
        }
    }
    return ok;
}

static bool w5500_write_reg(SPI_Handle spi, uint16_t addr, const uint8_t *buf, uint16_t len)
{
    SPI_Transaction trans;
    uint8_t txBuf[3 + 8] = {0};

    if (len > 8) return false;

    txBuf[0] = (addr >> 8) & 0xFF;
    txBuf[1] = addr & 0xFF;
    txBuf[2] = w5500_get_ctrl(addr, true);  // <-- FIX HERE
    for (int i = 0; i < len; i++) {
        txBuf[3 + i] = buf[i];
    }

    memset(&trans, 0, sizeof(trans));
    trans.count = 3 + len;
    trans.txBuf = txBuf;
    trans.rxBuf = NULL;

    GPIO_write(CONFIG_GPIO_SPI_CONTROLLER_CSN, 0);
    bool ok = SPI_transfer(spi, &trans);
    GPIO_write(CONFIG_GPIO_SPI_CONTROLLER_CSN, 1);

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