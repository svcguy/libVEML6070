/** @file veml6070.h
* 
* @brief    A driver for the VEML6070 UV Light Sensor by Vishay
*           https://www.vishay.com/ppg?84277
*           Based in part on the Adafruit VEML6070 Arduino Library
*           https://github.com/adafruit/Adafruit_VEML6070
*           Adafruit invests time and resources providing this open source code,
*           please support Adafruit and open-source hardware by purchasing
*           products from Adafruit!
*           Written by Limor Fried/Ladyada for Adafruit Industries.
*           BSD license, all text here must be included in any redistribution.
*
*           Adapted to be used with the STM32CubeMX libraries for STM32 microcontrollers
*           https://www.st.com/en/development-tools/stm32cubemx.html
*           
*           Author:  Andy Josephson, 2018
* @par      
*/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __veml6070_H
#define __veml6070_H

#include<stdbool.h>
#include<stdint.h>
#ifdef USE_HAL_DRIVER
#include "i2c.h"
#else
#error "The VEML6070 driver requires the use of STM32CubeMX HAL drivers"
#endif /* USE_HAL_DRIVER */

#define VEML6070_I2C_ADDRESS_HIGH       0x73
#define VEML6070_I2C_ADDRESS_LOW        0x71

#define VEML6070_I2C_TIMEOUT            100

/**************************************************************************/
/*! 
    @brief  integration time definitions
*/
/**************************************************************************/
typedef enum VEML6070_integrationtime 
{
  VEML6070_HALF_T,
  VEML6070_1_T,
  VEML6070_2_T,
  VEML6070_4_T,
} VEML6070_integrationtime_t;

/**************************************************************************/
/*! 
    @brief  command register definitions
*/
/**************************************************************************/
typedef union 
{
    struct 
    {
      uint8_t SD:1;
      uint8_t :1;
      uint8_t IT:2;
      uint8_t ACK_THD:1;
      uint8_t ACK:1;
    } bit;
    uint8_t reg;

  } VEML6070_commandRegister_t;

/**************************************************************************/
/*! 
    @brief  UV Index definitions
*/
/**************************************************************************/
enum UV_INDEX
{
    UV_INDEX_LOW = 0,
    UV_INDEX_MODERATE,
    UV_INDEX_HIGH,
    UV_INDEX_VERYHIGH,
    UV_INDEX_EXTREME,
    UV_INDEX_ITEM_COUNT
};

extern const uint16_t UV_INDEX_TABLE[3][UV_INDEX_ITEM_COUNT];
extern const char UV_INDEX_STRING[][10];

#ifdef __cplusplus
 extern "C" {
#endif

/**
 * @brief Initialization function
 * @param i2cHandle - A handle from the HAL I2C driver for the I2C instance
 * @param iTime - The integration time (sensor refresh time)
 * @retval false on failure, true on success
 */
bool VEML6070_init( I2C_HandleTypeDef i2cHandle, VEML6070_integrationtime_t iTime );
/**
 * @brief Writes a byte to the command register
 * @param cReg - The value to be written
 * @retval false on failure, true on success
 */
bool VEML6070_writeCommandReg( VEML6070_commandRegister_t cReg );
/**
 * @brief Sets the sleep state of the sensor
 * @param state - true to put the device to sleep, false to wake the device
 * @retval false on failure, true on success
 */
bool VEML6070_sleep( bool state );
/**
 * @brief Gets the UV reading
 * @param None
 * @retval 16-bit UV intensity value (unit not documented)
 */
uint16_t VEML6070_readUV( void );
/**
 * @brief Converts the 16-bit UV intensity value to UV Index
 * @param UV intensity reading
 * @retval UV Index (not the standard 1-11, but the enum UV_INDEX)
 */
uint8_t VEML6070_convertUVI(uint16_t uvReading );

#ifdef __cplusplus
}
#endif
#endif /*__veml6070_H */

// End of file veml6070.h