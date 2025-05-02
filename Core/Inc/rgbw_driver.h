#ifndef INC_RGBW_DRIVER_H_
#define INC_RGBW_DRIVER_H_

#include <stdint.h>
#include <stdbool.h>
#include "i2c_hw.h"

#include "stm32f3xx_hal.h"


#define RGBW_DRIVER_I2C_TIME_FOR_RESET_MS   1

typedef enum
{
    RGBW_CHIP_REG_RESET_CONTROL = 0x00,
    RGBW_CHIP_REG_CHANNEL_EN    = 0x04,
    RGBW_CHIP_REG_CHANNEL_RED   = 0x06,
    RGBW_CHIP_REG_CHANNEL_GREEN = 0x07,
    RGBW_CHIP_REG_CHANNEL_BLUE  = 0x08,
    RGBW_CHIP_REG_CHANNEL_WHITE = 0x09,
    RGBW_CHIP_REG_UNEXIST       = 0xff
} rgbw_chip_registers_t;

typedef enum{
    RED = 0,
    GREEN,
    BLUE,
    WHITE
}rgbw_driver_channels_t;

typedef enum{
    ALWAYS_OFF  = 0b00,
    ALWAYS_ON   = 0b01
}rgbw_mode_of_channel_t;

typedef enum{
    DO_NOTHING              = 0b100,
    RESET_REGISTERS_ONLY    = 0b101,
    RESET_COMPLATE          = 0b111,
    MODE_SHUTDOWN_ON_SLC_OR_SDA_LOW = 0b00000,
    MODE_ALWAYS_ON          = 0b11000
}rgbw_control_values_t;


i2chw_error_t rgbw_driver_init(
    const   i2chw_dev_t*        i2chw_dev,
    const   i2chw_cfg_t*        i2chw_cfg
);

i2chw_error_t rgbw_driver_set_channel_brightness(
    const   i2chw_dev_t *p_dev,
            rgbw_driver_channels_t channel,
            uint8_t brightness
);

i2chw_error_t rgbw_driver_set_rgb_color(
    const   i2chw_dev_t*    p_dev,
            uint8_t         red,
            uint8_t         green,
            uint8_t         blue
);

i2chw_error_t rgbw_driver_set_channel_mode(
    const   i2chw_dev_t*            p_dev,
            rgbw_driver_channels_t  channel,
            rgbw_mode_of_channel_t  mode
);

i2chw_error_t rgbw_driver_all_channels_activity(const i2chw_dev_t *p_dev, rgbw_mode_of_channel_t mode);

#endif
