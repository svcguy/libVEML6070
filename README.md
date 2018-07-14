# libVEML6070
STM32 CubeMX based library for the Vishay VEML6070 UV Light Sensor

This library uses the STM32 CubeMX HAL drivers to interface with the Vishay VEML6070 UV Light Sensor

STM32CubeMX - https://www.st.com/en/development-tools/stm32cubemx.html
VEML6070 - https://www.vishay.com/ppg?84277
VEML6070 Datasheet - https://www.vishay.com/docs/84277/veml6070.pdf
Vishay UV Index App Note - https://www.vishay.com/docs/84310/designingveml6070.pdf

I've adapted the code written by Limor Fried/Ladyada for the VEML6070 Arduino Library
https://github.com/adafruit/Adafruit_VEML6070

The library performs the following tasks:

* Can write the command register to setup the device
* Can read the UV intensity
* Can convert to UV Index based on the Vishay app note (for Rset = 270k)

The library is written in C
