#include "stm32f10x_spi.h"
#include "stm32f10x_gpio.h"

/**
 * @brief In this code i used to connect with w25q32
 * @note    CS - PA4
 *          CLK - PA5
 *          DO - PA6
 *          DI - PA7 
 */

void SPI1_Init(void);
uint16_t SPI1_transfer_w25q32(uint16_t data);
void CS_LOW(void);
void CS_HIGH(void);