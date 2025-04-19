#ifndef INC_RGBW_DRIVER_H_
#define INC_RGBW_DRIVER_H_

#include <stdint.h>

#include "stm32f3xx_hal.h"


#define RGBW_DRIVER_I2C_TIMEOUT_MS	100

typedef enum
{
	RGBW_CHIP_REG_ADRESS 		= 0x00,
	RGBW_CHIP_REG_ENABLED		= 0x01,
	RGBW_CHIP_REG_CHANNEL_RED	= 0x02,
	RGBW_CHIP_REG_CHANNEL_GREEN	= 0x03,
	RGBW_CHIP_REG_CHANNEL_BLUE	= 0x04,
	RGBW_CHIP_REG_CHANNEL_WHITE	= 0x05,
	RGBW_CHIP_REG_CNANNELS_MASK	= 0x06
} rgbw_chip_registers_t;

typedef enum{
	RED		= 0b0001,
	GREEN	= 0b0010,
	BLUE	= 0b0100,
	WHITE	= 0b1000
}rgbw_driver_channels_t;

extern uint8_t rgb_chip_i2c_adress;

HAL_StatusTypeDef rgbw_driver_init(I2C_HandleTypeDef* hi2c_dev);

HAL_StatusTypeDef rgbw_driver_set_channel_brightness(rgbw_driver_channels_t channel, uint8_t brightness);
HAL_StatusTypeDef rgbw_driver_set_rgb_color(uint8_t red, uint8_t green, uint8_t blue);

HAL_StatusTypeDef rgbw_driver_set_channel_activity(rgbw_driver_channels_t channel, bool activity);

HAL_StatusTypeDef rgbw_driver_all_channels_activity(bool enabled);

#endif
