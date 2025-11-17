#include "bdang_uart.h"

/**
 * @brief Init GPIOA Pin 9/10, init USART1
 * 
 * @note Both GPIO pin speed 50MHz, Pa9 AF PP, Pa10 Inp Floating
 */
void usart1_init(void){
    /* Init GPIOA, PA9-TX, PA10-RX --------------------------------- */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef gpioa_pin9_config = {
        .GPIO_Pin = GPIO_Pin_9,
        .GPIO_Speed = GPIO_Speed_50MHz,
        .GPIO_Mode = GPIO_Mode_AF_PP
    };
    GPIO_Init(GPIOA, &gpioa_pin9_config);

    GPIO_InitTypeDef gpioa_pin10_config = {
        .GPIO_Pin = GPIO_Pin_10,
        .GPIO_Speed = GPIO_Speed_50MHz,
        .GPIO_Mode = GPIO_Mode_IN_FLOATING
    };
    GPIO_Init(GPIOA, &gpioa_pin10_config);

    /* Init USART1 -------------------------------------------------- */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    USART_InitTypeDef usart1_config = {
        .USART_BaudRate = 115200,
        .USART_WordLength = USART_WordLength_8b,
        .USART_StopBits = USART_StopBits_1,
        .USART_Parity = USART_Parity_No,
        .USART_Mode = USART_Mode_Tx,
        .USART_HardwareFlowControl = USART_HardwareFlowControl_None
    };
    USART_Init(USART1, &usart1_config);
    USART_Cmd(USART1, ENABLE);
}

/**
 * @brief Send 1 character by USART_SendData of STL
 * 
 * @param c: Character need to send
 */
void usart1_send_char(char c) {
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, c);
}

/**
 * @brief Send a string by calling usart1_send_char
 * 
 * @note Send each byte by send char function
 * 
 * @param str: Pointer to the character array
 */
void usart1_send_string(char* str) {
    while(*str) {
        usart1_send_char(*str++);
    }
}