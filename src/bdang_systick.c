#include "bdang_systick.h"

/**
 * @brief   Init systick and update core clock
 */
void systick_init(uint32_t ticks){
    SystemCoreClockUpdate();
    if (SysTick_Config(SystemCoreClock / ticks) != 0) {
        while(1); 
    }
}   

/**
 * @brief   Interupt plus msTicks to 1
 */
void SysTick_Handler(void){
    msTicks++;
}

/**
 * @brief   Delay base on msTicks 
 * 
 * @param ms: Milisecond that you want to delay
 */
void delay_ms(uint32_t ms) {
    uint32_t start = msTicks;
    while ((msTicks - start) < ms);
}

/**
 * @brief   Get current msTicks
 * 
 * @retval  msTicks
 */
volatile uint32_t get_systick_ms(void){
    return msTicks;
}