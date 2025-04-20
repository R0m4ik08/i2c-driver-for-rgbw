#include <stdio.h>
#include "rgbw_driver.h"

int main(void)
{
    printf("Hello!\n");

    I2C_HandleTypeDef hi2c_dev;

    // Инициализация RGBW драйвера
    if (rgbw_driver_init(&hi2c_dev) != HAL_OK) {
        printf("Failed to initialize RGBW driver.\n");
        return -1;
    }
    printf("RGBW driver initialized successfully.\n");

    // Установка яркости каналов
    if (rgbw_driver_set_channel_brightness(RED, 128) == HAL_OK) {
        printf("Red channel brightness set to 128.\n");
    } else {
        printf("Failed to set red channel brightness.\n");
    }

    if (rgbw_driver_set_channel_brightness(GREEN, 64) == HAL_OK) {
        printf("Green channel brightness set to 64.\n");
    } else {
        printf("Failed to set green channel brightness.\n");
    }
    
    if (rgbw_driver_set_channel_brightness(BLUE, 255) == HAL_OK) {
        printf("Blue channel brightness set to 255.\n");
    } else {
        printf("Failed to set blue channel brightness.\n");
    }
    
    if (rgbw_driver_set_channel_brightness(WHITE, 0x20) == HAL_OK) {
        printf("White channel brightness set to 0x20.\n");
    } else {
        printf("Failed to set white channel brightness.\n");
    }

    // Установка активности каналов
    if (rgbw_driver_set_channel_activity(RED, true) == HAL_OK) {
        printf("Red channel activated.\n");
    } else {
        printf("Failed to activate red channel.\n");
    }

    if (rgbw_driver_set_channel_activity(RED, false) == HAL_OK) {
        printf("Red channel deactivated.\n");
    } else {
        printf("Failed to deactivate red channel.\n");
    }

    if (rgbw_driver_set_channel_activity(GREEN, true) == HAL_OK) {
        printf("Green channel activated.\n");
    } else {
        printf("Failed to activated green channel.\n");
    }
    
    if (rgbw_driver_set_channel_activity(BLUE, true) == HAL_OK) {
        printf("Blue channel activated.\n");
    } else {
        printf("Failed to activated blue channel.\n");
    }

    if (rgbw_driver_set_channel_activity(WHITE, true) == HAL_OK) {
        printf("White channel activated.\n");
    } else {
        printf("Failed to activated white channel.\n");
    }

    // Выключение всех каналов
    if (rgbw_driver_all_channels_activity(false) == HAL_OK) {
        printf("All channels deactivated.\n");
    } else {
        printf("Failed to deactivate all channels.\n");
    }

    if(rgbw_driver_set_rgb_color(0xff, 0x80, 0x40) == HAL_OK){
        printf("RGB color set successfully.\n");
    }else{
        printf("Failed to set RGB color.\n");
    }

    // Включение всех каналов
    if (rgbw_driver_all_channels_activity(true) == HAL_OK) {
        printf("All channels activated.\n");
    } else {
        printf("Failed to activate all channels.\n");
    }

    return 0;
}