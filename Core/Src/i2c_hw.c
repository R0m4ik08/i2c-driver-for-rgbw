#include "i2c_hw.h"

i2chw_error_t I2CHW_Init(const i2chw_bus_t bus_num)
{
#ifdef PC_DEBUG

    LOG("I2CHW_Init");

    LOG("i2chw_bus: %d", bus_num);

    LOG("-------------");

#endif // PC_DEBUG
    return I2CHW_SUCCESS;
}

i2chw_error_t I2CHW_Configure(const i2chw_bus_t bus_num, const i2chw_cfg_t *p_config)
{
#ifdef PC_DEBUG

    LOG("I2CHW_Configure:");

    LOG("i2chw_bus: %d", bus_num);
    LOG("config->bus_freq: %d", p_config->bus_freq);
    LOG("config->dir_mode: %d", p_config->dir_mode);

    LOG("-------------");

#endif // PC_DEBUG
    return I2CHW_SUCCESS;
}

i2chw_error_t I2CHW_WriteSync(const i2chw_dev_t *p_dev, const uint8_t *p_tx_data, const uint16_t tx_data_len)
{
#ifdef PC_DEBUG

    LOG("I2CHW_WriteSync");

    LOG("Dev:");
    LOG("\tdev->addr_width: %d", p_dev->addr_width);
    LOG("\tdev->dev_addr: 0x%04x", p_dev->dev_addr);
    LOG("\tdev->bus_num: %d", p_dev->bus_num);
    LOG("Tx_data:");
    for (int i = 0; i < tx_data_len; i++)
    {
        LOG("\t %d: 0x%02x", i, (uint8_t)*(p_tx_data + i));
    }

    LOG("-------------");

#endif // PC_DEBUG
    return I2CHW_SUCCESS;
}

i2chw_error_t I2CHW_WriteReadSync(const i2chw_dev_t *p_dev, const uint8_t *p_tx_data, const uint16_t tx_data_len, uint8_t *p_rx_data, const uint16_t rx_data_len)
{
#ifdef PC_DEBUG

    LOG("I2CHW_WriteReadSync");
    
    LOG("Dev:");
    LOG("\tdev->addr_width: %d", p_dev->addr_width);
    LOG("\tdev->dev_addr: 0x%04x", p_dev->dev_addr);
    LOG("\tdev->bus_num: %d", p_dev->bus_num);
    LOG("Tx_data:");
    for (int i = 0; i < tx_data_len; i++)
    {
        LOG("\t %d: 0x%02x", i, (uint8_t)*(p_tx_data + i));
    }

    // Возвращает значение со всеми выключенными каналами 0x00
    *p_rx_data = 0x55;

    LOG("-------------");

#endif // PC_DEBUG
    return I2CHW_SUCCESS;
}

i2chw_error_t I2CHW_ReadSync(const i2chw_dev_t *p_dev, uint8_t *p_rx_data, const uint16_t rx_data_len)
{
#ifdef PC_DEBUG

    LOG("I2CHW_ReadSync");
    
    LOG("Dev:");
    LOG("\tdev->addr_width: %d", p_dev->addr_width);
    LOG("\tdev->dev_addr: 0x%04x", p_dev->dev_addr);
    LOG("\tdev->bus_num: %d", p_dev->bus_num);

    // Возвращает значение со всеми выключенными каналами 0x00
    *p_rx_data = 0;

    LOG("-------------");

#endif // PC_DEBUG
    return I2CHW_SUCCESS;
}