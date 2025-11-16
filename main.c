#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

// Delay function
void delay_ms(uint32_t ms) {
    for(uint32_t i = 0; i < ms * 8000; i++) {
        __NOP();
    }
}

int main(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // Enable GPIOC clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    
    // Configure PC13 as output (Built-in LED)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    while(1) {
        // Toggle LED
        GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);  // LED ON
        delay_ms(500);
        
        GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);    // LED OFF
        delay_ms(500);
    }
    
    return 0;
}