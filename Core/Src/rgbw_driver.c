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

i2chw_error_t rgbw_driver_init(const i2chw_dev_t* i2chw_dev, const i2chw_cfg_t* i2chw_cfg )
{
    // Проверяет на ненулевые указатели

    if ( i2chw_dev == NULL || i2chw_cfg == NULL){
        return I2CHW_ERR_INVALID_PARAMS;
    }
    
    i2chw_error_t result;
    
    //  Конфигурирует i2c шину

    result = I2CHW_Configure(i2chw_dev->bus_num, i2chw_cfg);
    if (result != I2CHW_SUCCESS)
    {
        return result;
    }
    
    //  Инициализирует i2c шину

    result = I2CHW_Init(i2chw_dev->bus_num);
    if (result != I2CHW_SUCCESS)
    {
        return result;
    }

    //  Отправляет данные в регистр сброса/конфигурации для полного сброса состояния rgb-микросхемы (0b111)

    result = write_data_to_register_by_address(i2chw_dev, RGBW_CHIP_REG_RESET_CONTROL, (uint8_t) RESET_COMPLATE);

    //  //  Игнорирует ошибку отсутствия бита подтверждения

    if (result != I2CHW_ERR_NACK && result != I2CHW_SUCCESS)
    {
        return result;
    }

    //  Ожидает окончание процесса сброса состояния (200 мкс по описанию микросхемы) !!! По факту ожидает 1мс
    HAL_Delay(RGBW_DRIVER_I2C_TIME_FOR_RESET_MS); 

    //  Отправляет данные в регистр сброса/конфигурации для установки режима: всегда включен (0b11000)
    result = write_data_to_register_by_address(i2chw_dev, RGBW_CHIP_REG_RESET_CONTROL, (uint8_t) MODE_ALWAYS_ON);
    if (result != I2CHW_SUCCESS)
    {
        return result;
    }

    return result;
}

i2chw_error_t rgbw_driver_set_channel_brightness(const i2chw_dev_t *p_dev, rgbw_driver_channels_t channel, uint8_t brightness)
{
    //  Проверяет на ненулевой указатель устройства

    if ( p_dev == NULL){
        return I2CHW_ERR_INVALID_PARAMS;
    }
    
    //  Преобразует яркость из программного диапазона в аппаратный ([0..255] -> [0..190])
    
    uint8_t hardware_brightness = CONVERT_PROGRAM_RANGE_TO_HARDWARE(brightness);

    //  Определяет адрес регистра, в который будет записана яркость канала

    rgbw_chip_registers_t tmp_reg_addres;
    tmp_reg_addres = get_addres_from_channel(channel);
    if(tmp_reg_addres == RGBW_CHIP_REG_UNEXIST){
        return I2CHW_ERR_INVALID_PARAMS;
    }

    //  Отправляет данные в регистр соответствующего канала для установки переданной яркости

    i2chw_error_t result;
    result = write_data_to_register_by_address(p_dev, tmp_reg_addres, hardware_brightness);
    if (result != I2CHW_SUCCESS)
    {
        return result;
    }

    return result;
}

#pragma endregion



