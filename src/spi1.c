#include "spi1.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

/**
 * @brief Init GPIOA and SPI1
 * 
 * @retval None
 */
void SPI1_Init(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);

    /* GPIOA config ------------------------------------------------------------ */
    GPIO_InitTypeDef gpioa_pin4_config = {
        .GPIO_Pin = GPIO_Pin_4,
        .GPIO_Speed = GPIO_Speed_50MHz,
        .GPIO_Mode = GPIO_Mode_Out_PP
    };
    GPIO_InitTypeDef gpioa_pin57_config = {
        .GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7,
        .GPIO_Speed = GPIO_Speed_50MHz,
        .GPIO_Mode = GPIO_Mode_AF_PP
    };
    GPIO_InitTypeDef gpioa_pin6_config = {
        .GPIO_Pin = GPIO_Pin_6,
        .GPIO_Mode = GPIO_Mode_IN_FLOATING
    };
    GPIO_Init(GPIOA, &gpioa_pin4_config);                   // CS, push pull (HIGH/LOW)
    GPIO_Init(GPIOA, &gpioa_pin6_config);                   // MISO input floating                     
    GPIO_Init(GPIOA, &gpioa_pin57_config);                  // MOSI, SCLK AF PP


    /* SPI1 config ------------------------------------------------------------- */
    SPI_InitTypeDef spi1_config= {
        .SPI_Direction = SPI_Direction_2Lines_FullDuplex,
        .SPI_Mode = SPI_Mode_Master,
        .SPI_DataSize = SPI_DataSize_8b,
        .SPI_CPOL = SPI_CPOL_Low,
        .SPI_CPHA = SPI_CPHA_1Edge,
        .SPI_NSS = SPI_NSS_Soft,
        .SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4,
        .SPI_FirstBit = SPI_FirstBit_MSB,
        .SPI_CRCPolynomial = 7
    };
    SPI_Init(SPI1, &spi1_config);
    SPI_Cmd(SPI1, ENABLE);
}

/**
 * @brief Chip select, PA4 LOW/HIGH
 */
void CS_HIGH(void){
    GPIO_SetBits(GPIOA, GPIO_Pin_4);
}
void CS_LOW(void){
    GPIO_ResetBits(GPIOA, GPIO_Pin_4);
}

/**
 * @brief Transfer data and receive reply
 * 
 * @param data: Data that need to send
 * 
 * @retval Data from DR register after RXNE (not empty) is set
 */
uint16_t SPI1_transfer_w25q32(uint16_t data){
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, data);
    
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    return SPI_I2S_ReceiveData(SPI1);
}

/**
 * @brief Read Manufactor and ID of w25q32
 * 
 * @param manufactor: Pointer to a variable to save manufactor
 * @param id: Pointer to a variable to save id device
 */