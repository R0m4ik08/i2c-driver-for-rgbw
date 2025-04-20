#include "i2c.h"

rgbw_chip_t rgbw_chip = {
    .reg_addresses  = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06},
    .reg_values     = {0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    .address_reg_to_transmit = 0x00
};


HAL_StatusTypeDef check_device_address(uint8_t DevAddress){
    DevAddress >>= 1;
    if (DevAddress != rgbw_chip.reg_values[0]){
        LOG("Error: Device address mismatch. Expected: %02X, Received: %02X", rgbw_chip.reg_values[0], DevAddress);
        return HAL_ERROR;
    }
    return HAL_OK;
}

HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout){
    
    HAL_StatusTypeDef result = check_device_address(DevAddress);
    if (result != HAL_OK) {
        return result; // Возвращаем ошибку, если адрес устройства не совпадает
    }

    if (Size == 1){
        rgbw_chip.address_reg_to_transmit = pData[0];
        LOG("Address to transmit has been updated: %02X", rgbw_chip.address_reg_to_transmit);
        return HAL_OK; // Возвращаем успех, если передан только адрес
    }

    if (Size == 2){
        for (int i = 0; i < RGBW_CHIP_REGISTERS_COUNT; i++){
            if (pData[0] == rgbw_chip.reg_addresses[i]){
                rgbw_chip.reg_values[i] = pData[1];
                LOG("Register %02X updated to %02X", pData[0], pData[1]);
                return HAL_OK;
            }
        }
        return HAL_OK;
    }

    LOG("Error: Invalid size for I2C transmission. Size: %d", Size);
    return HAL_ERROR;
}
HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout){
    return HAL_OK;
}