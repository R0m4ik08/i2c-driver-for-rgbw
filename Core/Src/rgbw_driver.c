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

    result = I2CHW_WriteReadSync(i2chw_dev, (uint8_t*) &register_address, 1, data, 1);
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
#ifndef PC_DEBUG
    HAL_Delay(RGBW_DRIVER_I2C_TIME_FOR_RESET_MS);
#endif // PC_DEBUG

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

i2chw_error_t rgbw_driver_set_channel_mode( const i2chw_dev_t *p_dev, rgbw_driver_channels_t channel, rgbw_mode_of_channel_t mode )
{
    //  Проверяет на ненулевой указатель устройства

    if ( p_dev == NULL){
        return I2CHW_ERR_INVALID_PARAMS;
    }

    //  Запрашивает данные из регистра режимов работы RGBW каналов

    i2chw_error_t result;
    uint8_t reg_channel_en_data;
    result = read_data_from_register_by_address(p_dev, RGBW_CHIP_REG_CHANNEL_EN, &reg_channel_en_data);

    //  Изменяет полученные данные так, чтобы установить требуемый режим у соответствующего канала

    //  //  Обнуляет биты соответствующие режиму указанного канала 
    reg_channel_en_data &= (~(0b11 << (channel << 1))) & 0xff;

    //  //  Устанавливает новый режим работы для указанного канала
    reg_channel_en_data |= (mode <<  (channel << 1)) & 0xff;

    //  Отправляет новые данные в регистр режимов работы RGBW каналов

    result = write_data_to_register_by_address(p_dev, RGBW_CHIP_REG_CHANNEL_EN, reg_channel_en_data);
    if (result != I2CHW_SUCCESS)
    {
        return result;
    }


    return result;
}

i2chw_error_t rgbw_driver_all_channels_activity(const i2chw_dev_t *p_dev, rgbw_mode_of_channel_t mode)
{
    //  Проверяет на ненулевой указатель устройства

    if ( p_dev == NULL){
        return I2CHW_ERR_INVALID_PARAMS;
    }
    
    //Формирует данные для регистра RGBW_CHIP_REG_CHANNEL_EN, соответствующие установки всем каналам режима mode
    
    uint8_t data = 0;
    for (int i = 0; i < 4; i++)
    {
        data <<= 2;
        data  |= (uint8_t)mode & 0b11;
    }

    //  Отправляет подготовленные данные в регистр RGBW_CHIP_REG_CHANNEL_EN
    
    i2chw_error_t result;
    result = write_data_to_register_by_address(p_dev, RGBW_CHIP_REG_CHANNEL_EN, data);
    if (result != I2CHW_SUCCESS)
    {
        return result;
    }

    return result;
}

i2chw_error_t rgbw_driver_set_rgb_color(const i2chw_dev_t *p_dev, uint8_t red, uint8_t green, uint8_t blue)
{
    //  Проверяет на ненулевой указатель устройства

    if ( p_dev == NULL){
        return I2CHW_ERR_INVALID_PARAMS;
    }
   
    //  Получает составляющую белого цвета из переданных RGB значений
    uint8_t white_part = get_white_part_from_rgb(red, green, blue);
    
    //  Уменьшает RGB составляющие на величину белого цвета
    if(white_part > 0)
    {
        red     -= white_part;
        green   -= white_part;
        blue    -= white_part;
    }

    //  Преобразует яркость каналов из программного диапазона в аппаратный ([0..255] -> [0..190])

    white_part  = CONVERT_PROGRAM_RANGE_TO_HARDWARE(white_part);
    red         = CONVERT_PROGRAM_RANGE_TO_HARDWARE(red);
    green       = CONVERT_PROGRAM_RANGE_TO_HARDWARE(green);
    blue        = CONVERT_PROGRAM_RANGE_TO_HARDWARE(blue);

    //  Отправляет данные в регистры яркости RGBW каналов

    i2chw_error_t result;
    
    result = rgbw_driver_set_channel_brightness(p_dev, WHITE, white_part);
    if (result != I2CHW_SUCCESS)
    {
        return result;
    }

    result = rgbw_driver_set_channel_brightness(p_dev, RED, red);
    if (result != I2CHW_SUCCESS)
    {
        return result;
    }

    result = rgbw_driver_set_channel_brightness(p_dev, GREEN, green);
    if (result != I2CHW_SUCCESS)
    {
        return result;
    }

    result = rgbw_driver_set_channel_brightness(p_dev, BLUE, blue);
    if (result != I2CHW_SUCCESS)
    {
        return result;
    }

    //  Отправляет данные в регистр режимов работы RGBW каналов

    rgbw_mode_of_channel_t channel_mode;

    channel_mode = (white_part > 0) ? ALWAYS_ON : ALWAYS_OFF;
    result = rgbw_driver_set_channel_mode(p_dev, WHITE, channel_mode);
    if (result != I2CHW_SUCCESS)
    {
        return result;
    }
    
    channel_mode = (red > 0) ? ALWAYS_ON : ALWAYS_OFF;
    result = rgbw_driver_set_channel_mode(p_dev, RED, channel_mode);
    if (result != I2CHW_SUCCESS)
    {
        return result;
    }
    
    channel_mode = (green > 0) ? ALWAYS_ON : ALWAYS_OFF;
    result = rgbw_driver_set_channel_mode(p_dev, GREEN, channel_mode);
    if (result != I2CHW_SUCCESS)
    {
        return result;
    }
    
    channel_mode = (blue > 0) ? ALWAYS_ON : ALWAYS_OFF;
    result = rgbw_driver_set_channel_mode(p_dev, BLUE, channel_mode);
    if (result != I2CHW_SUCCESS)
    {
        return result;
    }

    return result;
}

#pragma endregion



