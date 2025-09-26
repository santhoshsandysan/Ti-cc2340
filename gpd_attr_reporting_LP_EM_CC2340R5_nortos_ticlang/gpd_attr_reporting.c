#include <ti/log/Log.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ti/drivers/GPIO.h>
#include "gboss_api.h"
#include "ti_zigbee_config.h"
#include "ti_drivers_config.h"
#include <ti/devices/DeviceFamily.h>

#define COMM_STEPS_LIMIT 100
#define COMMISSIONING_INTERVAL_MS 1000
#define REPORT_INTERVAL_MS 5000

// --- Device configuration ---
static const zb_uint8_t gs_commands[] = { GBOSS_APP_CMD_ID_LOCK_DOOR, GBOSS_APP_CMD_ID_UNLOCK_DOOR };
static const zb_uint16_t gs_clusters_cli[] = { ZB_ZCL_CLUSTER_ID_BASIC, ZB_ZCL_CLUSTER_ID_IDENTIFY, ZB_ZCL_CLUSTER_ID_SCENES, ZB_ZCL_CLUSTER_ID_ON_OFF, ZB_ZCL_CLUSTER_ID_DOOR_LOCK };
static const zb_uint16_t gs_clusters_srv[] = { ZB_ZCL_CLUSTER_ID_TEMP_MEASUREMENT, ZB_ZCL_CLUSTER_ID_REL_HUMIDITY_MEASUREMENT };

static const gboss_device_config_t g_device_config = {
    .manufacturer = 0x1234,
    .model        = 0x0001,
    .src_addr     = {0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88},
    .src_id       = GPD_SRC_ID,
    .application_id = GPD_APP_ID_TYPE,
    .endpoint     = 0x55,
    .device_id    = GBOSS_MANUF_SPECIFIC_DEV_ID,
    .comm_options = { .mac_sequence_number_capability = GBOSS_MAC_SEQ_NUM_INCREMENTAL,
                      .rx_on_capability = ZB_FALSE,
                      .application_info_present = ZB_TRUE,
                      .pan_id_request = ZB_FALSE,
                      .gp_security_key_request = ZB_FALSE,
                      .fixed_location = ZB_FALSE,
                      .extended_options_present = ZB_TRUE },
    .comm_extended_options = { .security_level_capabilities = SECURITY_LEVEL,
                               .key_type = SECURITY_KEY_TYPE,
                               .gpd_key_present = ZB_TRUE,
                               .gpd_key_encryption = ZB_TRUE,
                               .gpd_outgoing_counter_present = ZB_TRUE },
    .comm_app_info = { .manuf_id_present = ZB_TRUE,
                       .model_id_present = ZB_TRUE,
                       .gpd_commands_present = ZB_TRUE,
                       .cluster_list_present = ZB_TRUE,
                       .switch_info_present = ZB_FALSE,
                       .gpd_app_descr_command_follows = ZB_FALSE },
    .cmd_list_count = ZB_ARRAY_SIZE(gs_commands),
    .cmd_list       = gs_commands,
    .cluster_srv_cnt = ZB_ARRAY_SIZE(gs_clusters_srv),
    .cluster_list_srv = gs_clusters_srv,
    .cluster_cli_cnt = ZB_ARRAY_SIZE(gs_clusters_cli),
    .cluster_list_cli = gs_clusters_cli
};

// Preinstalled OOB key
static const zb_uint8_t g_gboss_encryption_key[] = SECURITY_KEY;
static const zb_uint8_t g_gboss_channels[] = CHANNEL_LIST;

// Transmit config
static const gboss_tx_cfg_t g_tx_cfg = {
    .channels = g_gboss_channels,
    .channel_count = ZB_ARRAY_SIZE(g_gboss_channels),
    .channel_group_size = 3,
    .repetitions = GPDF_REPETITIONS
};

// --- Attribute for string "27" ---
static zb_uint8_t attr_string_data[2] = { 0x32, 0x37 }; // ASCII '2','7'


// Example Power Config attributes
static zb_uint8_t attr_voltage = 33;
static zb_uint8_t attr_size = ZB_ZCL_POWER_CONFIG_BATTERY_SIZE_BUILT_IN;
static zb_uint8_t attr_quantity = 1;
static zb_uint8_t attr_rated_voltage = 40;
static zb_uint8_t attr_alarm_mask = 0;
static zb_uint8_t attr_voltage_min_threshold = 15;

ZB_ZCL_DECLARE_POWER_CONFIG_ATTRIB_LIST(power_config_battery_voltage_attrs,
                                        &attr_voltage,
                                        &attr_size,
                                        &attr_quantity,
                                        &attr_rated_voltage,
                                        &attr_alarm_mask,
                                        &attr_voltage_min_threshold);

// --- Delay function for NoRTOS ---
static void delay_ms(uint32_t ms)
{
    volatile uint32_t i;
    while(ms--)
        for(i = 0; i < 3000; i++) { /* crude approx delay */ }
}

// --- Main ---
MAIN()
{
    GPIO_setConfig(CONFIG_GPIO_GLED, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    printf("Green Power Device Starting...\n");

    zb_uint_t comm_steps = 0;

    // Platform init
    gboss_platform_init(ARGC_ARGV);
    gboss_app_init(g_gboss_encryption_key, &g_device_config);

#if (UNIDIR_DEVICE == ZB_FALSE)
    gboss_bidir_operational_init(&g_device_config, &g_tx_cfg);

#if (BATTERYLESS_DEVICE == ZB_TRUE)
    gboss_bidir_batteryless_mode();
#else
    gboss_bidir_battery_mode();
#endif
#endif

    // --- Commissioning loop ---
while (!gboss_bidir_is_commissionned())
{
    // Send as raw byte array using GBOSS_CMD_ID_ATTR_REPORT
    GBOSS_CMD_ID_ATTR_REPORT(&g_device_config,
                             &g_tx_cfg,
                             ZB_ZCL_CLUSTER_ID_BASIC,  // test cluster
                             2,                        // number of bytes
                             attr_string_data);        // pointer to bytes

    // printf("Commissioning attempt sending data: 27\n");

    gboss_bidir_commissioning_step(&g_device_config, &g_tx_cfg);
    GPIO_toggle(CONFIG_GPIO_GLED);
    delay_ms(COMMISSIONING_INTERVAL_MS);
    
    if(comm_steps > COMM_STEPS_LIMIT)
        {
            printf("Commissioning failed after %d attempts.\n", COMM_STEPS_LIMIT);
            break;
        }
    }

    printf("Device commissioned!\n");

    // --- Periodic Power Config reporting ---
    while (1)
    {
        GBOSS_CMD_ID_ATTR_REPORT(&g_device_config,
                                 &g_tx_cfg,
                                 ZB_ZCL_CLUSTER_ID_POWER_CONFIG,
                                 ZB_ZCL_ATTRIB_LIST_GET_COUNT(power_config_battery_voltage_attrs),
                                 ZB_ZCL_ATTRIB_LIST_GET_PTR(power_config_battery_voltage_attrs));

        printf("Power Config attribute report sent.\n");
        GPIO_toggle(CONFIG_GPIO_GLED);
        delay_ms(REPORT_INTERVAL_MS);
    }
}
