#include "stm32f10x_spi.h"
#include "stm32f10x_gpio.h"

void SPI1_Init(void);
uint16_t SPI1_transfer_w25q32(uint16_t data);
void CS_LOW(void);
void CS_HIGH(void);