#ifndef INC_RGBW_DRIVER_H_
#define INC_RGBW_DRIVER_H_

#include <stdint.h>

#include "stm32f3xx_hal.h"

typedef enum{
	RED		= 0b1,
	GREEN	= 0b10,
	BLUE	= 0b100,
	WHITE	= 0b1000
}rgbw_driver_channels_t;

extern uint8_t rgb_chip_i2c_adress;

HAL_StatusTypeDef rgbw_driver_init(I2C_HandleTypeDef* hi2c_dev);

HAL_StatusTypeDef rgbw_driver_set_chanal_brightness(rgbw_driver_channels_t channel, uint8_t brightness);
HAL_StatusTypeDef rgbw_driver_set_rgb_color(uint8_t red, uint8_t green, uint8_t blue);

HAL_StatusTypeDef rgbw_driver_channel_enable(rgbw_driver_channels_t channel);
HAL_StatusTypeDef rgbw_driver_channel_disable(rgbw_driver_channels_t channel);

HAL_StatusTypeDef rgbw_driver_all_channel_enable();
HAL_StatusTypeDef rgbw_driver_all_channel_disable();

#endif
