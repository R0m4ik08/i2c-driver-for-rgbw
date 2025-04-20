#ifndef I2C_H
#define I2C_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef DEBUG
    #define DEBUG 0
#endif

// Макрос для вывода отладочной информации
#if DEBUG
    #ifndef DEBUG_PRINT
        #define DEBUG_PRINT printf
    #endif
    #define LOG(fmt, ...) DEBUG_PRINT("[DEBUG] " fmt "\n", ##__VA_ARGS__)
#else
    #define LOG(fmt, ...)
#endif

/** 
  * @brief  HAL Status structures definition  
  */  
 typedef enum 
 {
   HAL_OK       = 0x00U,
   HAL_ERROR    = 0x01U,
   HAL_BUSY     = 0x02U,
   HAL_TIMEOUT  = 0x03
 } HAL_StatusTypeDef;


typedef struct __I2C_HandleTypeDef
{
   uint32_t Instance;  /*!< I2C registers base address */
   uint32_t Init;      /*!< I2C communication parameters */
   uint32_t State;     /*!< I2C communication state */
   uint32_t Lock;      /*!< Locking object */
   uint32_t ErrorCode; /*!< I2C Error code */
   void (*MspInitCallback)(struct __I2C_HandleTypeDef *hi2c);  /*!< I2C Msp Init callback */
   void (*MspDeInitCallback)(struct __I2C_HandleTypeDef *hi2c);/*!< I2C Msp DeInit callback */
} I2C_HandleTypeDef;

#define RGBW_CHIP_REGISTERS_COUNT 7
typedef struct{
   uint8_t reg_addresses[RGBW_CHIP_REGISTERS_COUNT]; 
   uint8_t reg_values[RGBW_CHIP_REGISTERS_COUNT];
   uint8_t address_reg_to_transmit;
}rgbw_chip_t;

/* IO operation functions  ****************************************************/
/******* Blocking mode: Polling */
HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData,
    uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData,
   uint16_t Size, uint32_t Timeout);

#endif /* I2C_H */