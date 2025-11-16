#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "bdang_systick.h"

volatile uint32_t msTicks = 0;

int main(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    systick_init(1000);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    while(1) {
        GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);  // LED ON
        delay_ms(1000);
        
        GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);    // LED OFF
        delay_ms(1000);
    }   
    return 0;
}