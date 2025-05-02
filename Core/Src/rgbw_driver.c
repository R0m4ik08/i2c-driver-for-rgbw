#include "rgbw_driver.h"

#pragma region Private functions

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
            return RGBW_CHIP_REG_UNEXIST;
    }
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

i2chw_error_t write_data_to_register_by_address(const i2chw_dev_t* i2chw_dev, rgbw_chip_registers_t register_address, uint8_t data)
{
    //  Проверяет на ненулевой указатель устройства

    if (i2chw_dev == NULL)
    {
        return I2CHW_ERR_INVALID_PARAMS;
    }
    
    i2chw_error_t result;

    //  Отправляет адрес rgb-микросхемы расширенный до 8 бит  

    uint8_t dev_address_with_command_bit = (i2chw_dev->dev_addr << 1);

    result = I2CHW_WriteSync(i2chw_dev, &dev_address_with_command_bit, 1);
    if (result != I2CHW_SUCCESS)
    {
        return result;
    }

    //  Отправляет адрес регистра

    result = I2CHW_WriteSync(i2chw_dev, (uint8_t*) &register_address, 1);
    if (result != I2CHW_SUCCESS)
    {
        return result;
    }

    //  Отправляет данные которые должны быть записаны в указанный регистр

    result = I2CHW_WriteSync(i2chw_dev, (uint8_t*) &data, 1);
    if (result != I2CHW_SUCCESS)
    {
        return result;
    }

    return result;
}

i2chw_error_t read_data_from_register_by_address(const i2chw_dev_t* i2chw_dev, rgbw_chip_registers_t register_address, uint8_t* data)
{
    //  Проверяет на ненулевой указатель устройства

    if (i2chw_dev == NULL)
    {
        return I2CHW_ERR_INVALID_PARAMS;
    }
    
    i2chw_error_t result;
    
    //  Отправляет адрес rgb-микросхемы расширенный до 8 бит  

    uint8_t dev_address_with_command_bit = (i2chw_dev->dev_addr << 1) | 0b1;

    result = I2CHW_WriteSync(i2chw_dev, &dev_address_with_command_bit, 1);
    if (result != I2CHW_SUCCESS)
    {
        return result;
    }

    //  Отправляет адрес регистра и получает его данные, которые должна отправить rgb-микросхема

    result = I2CHW_WriteReadSync(i2chw_dev, (uint8_t*) &register_address, 1, &data, 1);
    if (result != I2CHW_SUCCESS)
    {
        return result;
    }
    
    return result;
}

#pragma endregion

#pragma region Public functions
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

#pragma endregion



