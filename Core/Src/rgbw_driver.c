#include "rgbw_driver.h"

uint8_t rgb_chip_i2c_adress = 0x0A;

I2C_HandleTypeDef* rgbw_driver_hi2c_dev = NULL;

rgbw_chip_registers_t get_addres_from_channel(rgbw_driver_channels_t channel){

    switch(channel){
        case RED:
            return RGBW_CHIP_REG_CHANNEL_RED;
        case GREEN:
            return RGBW_CHIP_REG_CHANNEL_GREEN;
        case BLUE:
            return RGBW_CHIP_REG_CHANNEL_BLUE;
        case WHITE:
            return RGBW_CHIP_REG_CHANNEL_WHITE;
        default:
            return RGBW_CHIP_REG_INCORRECT;
    }
}

HAL_StatusTypeDef get_current_channels_state(uint8_t* current_state) {
    HAL_StatusTypeDef result = HAL_ERROR;
    rgbw_chip_registers_t tmp_reg_addres = RGBW_CHIP_REG_CHANNELS_MASK;

    // Запрашиваем текущее состояние каналов
    result = HAL_I2C_Master_Transmit(rgbw_driver_hi2c_dev, (rgb_chip_i2c_adress << 1), (uint8_t*)(&tmp_reg_addres), 1, RGBW_DRIVER_I2C_TIMEOUT_MS);
    if (result != HAL_OK) {
        return result;
    }

    result = HAL_I2C_Master_Receive(rgbw_driver_hi2c_dev, (rgb_chip_i2c_adress << 1), current_state, 1, RGBW_DRIVER_I2C_TIMEOUT_MS);
    if (result != HAL_OK) {
        return result;
    }

    return result;
}

uint8_t get_white_part_from_rgb(uint8_t red, uint8_t green, uint8_t blue){
    uint8_t white_part = red;
    if(green < white_part){
        white_part = green;
    }
    if(blue < white_part){
        white_part = blue;
    }
    return white_part;
}


HAL_StatusTypeDef rgbw_driver_init(I2C_HandleTypeDef* hi2c_dev){

    rgbw_chip_registers_t tmp_reg_addres;
    uint8_t tmp_data[2];

    // Сохраняем указатель на I2C

    rgbw_driver_hi2c_dev = hi2c_dev;

    // Включаем микросхему

    tmp_reg_addres = RGBW_CHIP_REG_ENABLED;
    tmp_data[0] = (uint8_t) tmp_reg_addres;
    tmp_data[1] = 0x01;

    HAL_StatusTypeDef transmitting_result = HAL_I2C_Master_Transmit(hi2c_dev, (rgb_chip_i2c_adress << 1), tmp_data, 2, RGBW_DRIVER_I2C_TIMEOUT_MS);
    if(transmitting_result != HAL_OK){
        return transmitting_result;
    }

    // Гасим все каналы

    tmp_reg_addres = RGBW_CHIP_REG_CHANNELS_MASK;
    tmp_data[0] = (uint8_t) tmp_reg_addres;
    tmp_data[1] = 0x00;

    transmitting_result = HAL_I2C_Master_Transmit(hi2c_dev, (rgb_chip_i2c_adress << 1), tmp_data, 2, RGBW_DRIVER_I2C_TIMEOUT_MS);
    if(transmitting_result != HAL_OK){
        return transmitting_result;
    }

    return transmitting_result;
}

HAL_StatusTypeDef rgbw_driver_set_channel_brightness(rgbw_driver_channels_t channel, uint8_t brightness){
    // Проверка на то, что была ли инициализирована микросхема управления RGBW
    if(rgbw_driver_hi2c_dev == NULL){
        return HAL_ERROR;
    }

    HAL_StatusTypeDef result =  HAL_ERROR;
    rgbw_chip_registers_t tmp_reg_addres;
    uint8_t tmp_data[2];

    // Определяем адрес регистра, в который будем записывать яркость канала
    tmp_reg_addres = get_addres_from_channel(channel);
    if(tmp_reg_addres == RGBW_CHIP_REG_INCORRECT){
        return HAL_ERROR;
    }

    // Формируем данные(2 байта) для передачи по I2C

    tmp_data[0] = (uint8_t) tmp_reg_addres;
    tmp_data[1] = brightness;

    // Отправляем данные по I2C

    result = HAL_I2C_Master_Transmit(rgbw_driver_hi2c_dev, (rgb_chip_i2c_adress << 1), tmp_data, 2, RGBW_DRIVER_I2C_TIMEOUT_MS);
    if(result != HAL_OK){
        return result;
    }
    
    return result;
}

HAL_StatusTypeDef rgbw_driver_set_channel_activity(rgbw_driver_channels_t channel, bool activity){
    // Проверка на то, что была ли инициализирована микросхема управления RGBW
    if(rgbw_driver_hi2c_dev == NULL){
        return HAL_ERROR;
    }

    HAL_StatusTypeDef result =  HAL_ERROR;
    rgbw_chip_registers_t tmp_reg_addres = RGBW_CHIP_REG_CHANNELS_MASK;
    uint8_t tmp_data[2];

    // Запрашиваем текущее состояние каналов
    result = get_current_channels_state(&tmp_data[1]);
    if(result != HAL_OK){
        return result;
    }

    // Формируем данные(2 байта) для передачи по I2C

    tmp_data[0]  = (uint8_t) tmp_reg_addres;

    if (activity){
        tmp_data[1] |= (uint8_t) channel;	
    } else {
        tmp_data[1] &= (uint8_t) (~channel);
    }	

    // Отправляем данные по I2C

    result = HAL_I2C_Master_Transmit(rgbw_driver_hi2c_dev, (rgb_chip_i2c_adress << 1), tmp_data, 2, RGBW_DRIVER_I2C_TIMEOUT_MS);
    if(result != HAL_OK){
        return result;
    }

    return result;
}

HAL_StatusTypeDef rgbw_driver_all_channels_activity(bool enabled){
    // Проверка на то, что была ли инициализирована микросхема управления RGBW
    if(rgbw_driver_hi2c_dev == NULL){
        return HAL_ERROR;
    }

    HAL_StatusTypeDef result =  HAL_ERROR;
    rgbw_chip_registers_t tmp_reg_addres = RGBW_CHIP_REG_CHANNELS_MASK;
    uint8_t tmp_data[2];

    // Формируем данные(2 байта) для передачи по I2C

    tmp_data[0] = (uint8_t) tmp_reg_addres;
    uint8_t all_channels_mask = (RED | GREEN | BLUE | WHITE);
    tmp_data[1] = enabled ? all_channels_mask : 0x00; 

    // Отправляем данные по I2C

    result = HAL_I2C_Master_Transmit(rgbw_driver_hi2c_dev, (rgb_chip_i2c_adress << 1), tmp_data, 2, RGBW_DRIVER_I2C_TIMEOUT_MS);
    if(result != HAL_OK){
        return result;
    }

    return result;
}

HAL_StatusTypeDef rgbw_driver_set_rgb_color(uint8_t red, uint8_t green, uint8_t blue){
    // Проверка на то, что была ли инициализирована микросхема управления RGBW
    if(rgbw_driver_hi2c_dev == NULL){
        return HAL_ERROR;
    }

    // Получаем составляющую белого цвета из переданных RGB значений
    uint8_t white_part = get_white_part_from_rgb(red, green, blue);
    
    // Устанавливаем яркость белого канала и уменьшаем RGB составляющие на величину белого цвета
    if(white_part > 0){
        rgbw_driver_set_channel_brightness(WHITE, white_part);
        red     -= white_part;
        green   -= white_part;
        blue    -= white_part;
    }

    HAL_StatusTypeDef result;
    result = rgbw_driver_set_channel_brightness(RED, red);
    if(result != HAL_OK){
        return result;
    }

    result = rgbw_driver_set_channel_brightness(GREEN, green);
    if(result != HAL_OK){
        return result;
    }

    result = rgbw_driver_set_channel_brightness(BLUE, blue);
    if(result != HAL_OK){
        return result;
    }

    return result;
}



