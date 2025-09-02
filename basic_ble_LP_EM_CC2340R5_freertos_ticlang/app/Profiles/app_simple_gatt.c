/******************************************************************************

@file  app_simple_gatt.c

@brief This file contains the Simple GATT application functionality

Group: WCS, BTS
Target Device: cc23xx

******************************************************************************

 Copyright (c) 2022-2025, Texas Instruments Incorporated
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 *  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 *  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 *  Neither the name of Texas Instruments Incorporated nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

******************************************************************************


*****************************************************************************/

//*****************************************************************************
//! Includes
//*****************************************************************************
#include <string.h>
#include "ti/ble/app_util/framework/bleapputil_api.h"
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Board.h>
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>

#include <ti/ble/profiles/simple_gatt/simple_gatt_profile.h>
#include <ti/drivers/Temperature.h>
#include "ti/ble/app_util/menu/menu_module.h"
#include <app_main.h>
#include "ti_drivers_config.h"
#include <FreeRTOS.h>
#include <timers.h>


#include <ti/drivers/GPIO.h>
#include <ti/drivers/SPI.h>

//*****************************************************************************
//! Defines
//*****************************************************************************

//*****************************************************************************
//! Globals
//*****************************************************************************

#define SIMPLEGATTPROFILE_CHAR1_LEN 2


static void SimpleGatt_changeCB( uint8_t paramId );

// Simple GATT Profile Callbacks
static SimpleGattProfile_CBs_t simpleGatt_profileCBs =
{
  SimpleGatt_changeCB // Simple GATT Characteristic value change callback
};


static TimerHandle_t tempTimerHandle;

int16_t currentTemperature;



//*****************************************************************************
//! Functions
//*****************************************************************************

/*********************************************************************
 * @fn      SimpleGatt_ChangeCB
 *
 * @brief   Callback from Simple Profile indicating a characteristic
 *          value change.
 *
 * @param   paramId - parameter Id of the value that was changed.
 *
 * @return  None.
 */
static void SimpleGatt_changeCB( uint8_t paramId )
{
  uint8_t newValue = 0;

  switch( paramId )
  {
    case SIMPLEGATTPROFILE_CHAR1:
      {
        SimpleGattProfile_getParameter( SIMPLEGATTPROFILE_CHAR1, &newValue );

        // Print the new value of char 1
        MenuModule_printf(APP_MENU_PROFILE_STATUS_LINE, 0, "Profile status: Simple profile - "
                          "Char 1 value = " MENU_MODULE_COLOR_YELLOW "%d " MENU_MODULE_COLOR_RESET,
                          newValue);
      }
      break;

    case SIMPLEGATTPROFILE_CHAR2:
      {
        SimpleGattProfile_getParameter(SIMPLEGATTPROFILE_CHAR2, &newValue);

        // Print the new value of char 2
        MenuModule_printf(APP_MENU_PROFILE_STATUS_LINE, 0, "Profile status: Simple profile - "
                          "Char 2 value = " MENU_MODULE_COLOR_YELLOW "%d " MENU_MODULE_COLOR_RESET,
                          newValue);

      }
      break;

    case SIMPLEGATTPROFILE_CHAR3:
      {
        SimpleGattProfile_getParameter(SIMPLEGATTPROFILE_CHAR3, &newValue);

        // Print the new value of char 3
        MenuModule_printf(APP_MENU_PROFILE_STATUS_LINE, 0, "Profile status: Simple profile - "
                          "Char 3 value = " MENU_MODULE_COLOR_YELLOW "%d " MENU_MODULE_COLOR_RESET,
                          newValue);

      }
      break;
    case SIMPLEGATTPROFILE_CHAR4:
      {
          // Print Notification registration to user
          SimpleGattProfile_getParameter(SIMPLEGATTPROFILE_CHAR4, &newValue);
          MenuModule_printf(APP_MENU_PROFILE_STATUS_LINE, 0, "Profile status: Simple profile - "
                                    "Char 4 = Notification registration");

          // SimpleGatt_notifyChar4();
          break;
      }

 case SIMPLEGATTPROFILE_CHAR6:
{
    SimpleGattProfile_getParameter(SIMPLEGATTPROFILE_CHAR6, &newValue);

    MenuModule_printf(APP_MENU_PROFILE_STATUS_LINE, 0,
                      "Profile status: Simple profile - "
                      "Char 6 value = " MENU_MODULE_COLOR_YELLOW "%d " MENU_MODULE_COLOR_RESET,
                      newValue);

    // 👉 Action on CHAR6 write
    if(newValue == 1)
    {
        GPIO_write(CONFIG_GPIO_LED_GREEN, 1);  // Turn ON LED
    }
    else if(newValue == 0)
    {
        GPIO_write(CONFIG_GPIO_LED_GREEN, 0);  // Turn OFF LED
    }
    else
    {
        // You can handle other values if needed
        MenuModule_printf(APP_MENU_PROFILE_STATUS_LINE, 0,
                          "Invalid Char6 value received: %d", newValue);
    }
}
break;

    default:
      // should not reach here!
      break;
  }
}

/*********************************************************************
 * @fn      SimpleGatt_start
 *
 * @brief   This function is called after stack initialization,
 *          the purpose of this function is to initialize and
 *          register the Simple GATT profile.
 *
 * @return  SUCCESS or stack call status
 */
bStatus_t SimpleGatt_start( void )
{
  bStatus_t status = SUCCESS;
      GPIO_init();
    
  // Add Simple GATT service
  status = SimpleGattProfile_addService();
  if (status != SUCCESS)
  {
	// Return status value
    return(status);
  }

  // Setup the Simple GATT Characteristic Values
  // For more information, see the GATT and GATTServApp sections in the User's Guide:
  // http://software-dl.ti.com/lprf/ble5stack-latest/
  {

    Temperature_init();

    currentTemperature = Temperature_getTemperature();
    GPIO_toggle(CONFIG_GPIO_LED_GREEN);
    uint8_t charValue1 = currentTemperature;
    uint8_t charValue2 = 2;
    uint8_t charValue3 = 3;
    uint8_t charValue4 = 4;
    uint8_t charValue5[SIMPLEGATTPROFILE_CHAR5_LEN] = { 1, 2, 3, 4, 5 };
    uint8_t charValue6 = 6;

SimpleGattProfile_setParameter(SIMPLEGATTPROFILE_CHAR6, sizeof(uint8_t),
                                &charValue6);


    SimpleGattProfile_setParameter( SIMPLEGATTPROFILE_CHAR1, sizeof(uint8_t),
                                    &currentTemperature );
    SimpleGattProfile_setParameter( SIMPLEGATTPROFILE_CHAR2, sizeof(uint8_t),
                                    &charValue2 );
    SimpleGattProfile_setParameter( SIMPLEGATTPROFILE_CHAR3, sizeof(uint8_t),
                                    &charValue3 );
    SimpleGattProfile_setParameter( SIMPLEGATTPROFILE_CHAR4, sizeof(uint8_t),
                                    &charValue4 );
    SimpleGattProfile_setParameter( SIMPLEGATTPROFILE_CHAR5, SIMPLEGATTPROFILE_CHAR5_LEN,
                                    charValue5 );
  }
  // Register callback with SimpleGATTprofile
  status = SimpleGattProfile_registerAppCBs( &simpleGatt_profileCBs );
  // Create FreeRTOS software timer for periodic temperature read (1s)


  // Return status value
  return(status);
}

/*********************************************************************
 * @fn      SimpleGatt_notifyChar4
 *
 * @brief   This function is called when WriteReq has been received to Char 4 or to Char 3.
 *          The purpose of this function is to send notification of Char 3 with the value
 *          of Char 3.
 *
 * @return  void
 */

 
