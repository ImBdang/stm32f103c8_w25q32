#include "bdang_systick.h"

void systick_init(uint32_t ticks){
    SystemCoreClockUpdate();
    if (SysTick_Config(SystemCoreClock / ticks) != 0) {
        while(1); 
    }
}   

void SysTick_Handler(void){
    msTicks++;
}

void delay_ms(uint32_t ms) {
    uint32_t start = msTicks;
    while ((msTicks - start) < ms);
}

volatile uint32_t get_systick_ms(void){
    return msTicks;
}