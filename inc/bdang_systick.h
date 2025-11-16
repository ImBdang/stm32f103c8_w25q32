#include "stm32f10x.h"
#include "core_cm3.h"
#include "system_stm32f10x.h"
#include "stdint.h"


extern volatile uint32_t msTicks;

void systick_init(uint32_t ticks);
void delay_ms(uint32_t ms);
volatile uint32_t get_systick_ms(void);