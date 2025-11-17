#ifndef __BDANGUART__
#define __BDANGUART__

#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

void usart1_init(void);
void usart1_send_char(char c);
void usart1_send_string(char* str);

#endif