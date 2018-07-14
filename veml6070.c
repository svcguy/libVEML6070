/** @file veml6070.c
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

#include "veml6070.h"

/**************************************************************************/
/*! 
    @brief  UV Index Table.  This table is valid only for Rset = 270k.  The
            first index is the integration time parameter, IT.  The second
            represents the high limit of the range for that value
*/
/**************************************************************************/
const uint16_t UV_INDEX_TABLE[3][UV_INDEX_ITEM_COUNT] = { { 560, 1120, 1494, 2054, 65535 },
                                                        { 1120, 2241, 2988, 4108, 65535 },
                                                        { 2240, 4482, 5976, 8216, 65535 } };

const char UV_INDEX_STRING[][10] = { "LOW", "MODERATE", "HIGH", "VERY HIGH", "EXTREME" };

I2C_HandleTypeDef _i2cHandle;
VEML6070_commandRegister_t _commandRegister;

bool VEML6070_init( I2C_HandleTypeDef i2cHandle, VEML6070_integrationtime_t iTime )
{
    // Copy reference to i2c HAL driver
    _i2cHandle = i2cHandle;
    // Set inital values in command register (datasheet pg.7)
    _commandRegister.reg = 0x02;
    // Set integration time
    _commandRegister.bit.IT = iTime;
    // Write the command register
    if( !VEML6070_writeCommandReg( _commandRegister ) )
    {
        return false;
    }

    return true;
}

bool VEML6070_writeCommandReg( VEML6070_commandRegister_t cReg )
{
    HAL_StatusTypeDef ret;
    // I2C transmission
    ret = HAL_I2C_Master_Transmit( &_i2cHandle, (uint16_t)VEML6070_I2C_ADDRESS_LOW, (uint8_t *)&_commandRegister, 1, VEML6070_I2C_TIMEOUT );
    
    if( ret != HAL_OK )
    {
        return false;
    }
    else
    {
        return true;
    }
}
bool VEML6070_sleep( bool state )
{
    _commandRegister.bit.SD = state;
    // Write command register
    if( !VEML6070_writeCommandReg( _commandRegister ) )
    {
        return false;
    }
    else
    {
        return true;
    }
}

uint16_t VEML6070_readUV( void )
{
    HAL_StatusTypeDef ret;
    uint8_t data;
    uint16_t reading;

    // Get high byte
    ret = HAL_I2C_Master_Receive( &_i2cHandle, (uint16_t)VEML6070_I2C_ADDRESS_HIGH, &data, 1, VEML6070_I2C_TIMEOUT );

    if( ret != HAL_OK )
    {
        return -1;
    }

    reading = data << 8;

    // Get low byte
    ret = HAL_I2C_Master_Receive( &_i2cHandle, (uint16_t)VEML6070_I2C_ADDRESS_LOW, &data, 1, VEML6070_I2C_TIMEOUT );

    if( ret != HAL_OK )
    {
        return -1;
    }

    reading += data;

    return reading;
}

uint8_t VEML6070_convertUVI(uint16_t uvReading )
{
    uint8_t i = 0;
    uint8_t index;

    // The tables provided in the app note are based on the integration time
    // Column0 = IT1
    // Column1 = IT2
    // therefore we must adjust the index
    if( _commandRegister.bit.IT <= 1 )
    {
        index = 0;
    }
    else
    {
        index = _commandRegister.bit.IT - 1;
    }

    // Traverse through the table till we get the approriate value
    while( uvReading > UV_INDEX_TABLE[index][i] )
    {
        i++;
    }

    return i;
}
 
 // End of file veml6070.c