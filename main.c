#include "stdio.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_exti.h"
#include "misc.h"
#include "stm32f1xx_it.h"

#include "bdang_systick.h"
#include "w25q32.h"
#include "spi1.h"
#include "bdang_ds1307.h"
#include "bdang_uart.h"

/* Declaration zone ------------------------------------------------------ */
void SystemClock_Config(void);
void Init_LedPc13(void);
void Test_W25Q32_WriteRead(char* str, uint32_t addr);
void Init_EXTI(void);


/* Global variables ------------------------------------------------------ */
volatile uint32_t msTicks = 0;
uint32_t next_addr;

/* MAIN function ---------------------------------------------------------- */
int main(void) {

    SystemClock_Config();
    systick_init(1000);
    SPI1_Init();
    W25Q32_EraseSector(0x000000);
    W25Q32_EraseSector(0x002000);
    initCurrentAddr();
    next_addr = readCurrentAddr();
    Init_LedPc13();
    usart1_init();
    Init_EXTI();
    init_i2c1();
    ds1307_enable();
    // uint8_t time[6];
    while(1) {
        // ds1307_gettime(time);
        // uint8_t sec  = bcd2dec(time[0] & 0x7F);  
        // uint8_t min  = bcd2dec(time[1]);
        // uint8_t hour = bcd2dec(time[2] & 0x3F);  
        // uint8_t day  = time[3];
        // uint8_t date = bcd2dec(time[4]);
        // uint8_t mon  = bcd2dec(time[5]);
        // uint8_t year = bcd2dec(time[6]);
        // char str[26];
        // snprintf(str, sizeof(str),"User pressed at %02u:%02u:%02u\n",hour, min, sec);
        // Test_W25Q32_WriteRead(str, next_addr);
    }
    return 0;
}

/* Define zone -------------------------------------------------------------------- */
/**
 * @brief   72Mhz, HSE as source
 */
void SystemClock_Config(void) {
    ErrorStatus HSEStartUpStatus;

    RCC_DeInit();
    RCC_HSEConfig(RCC_HSE_ON);
    HSEStartUpStatus = RCC_WaitForHSEStartUp();
    
    if(HSEStartUpStatus == SUCCESS) {
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
        FLASH_SetLatency(FLASH_Latency_2);
        
        RCC_HCLKConfig(RCC_SYSCLK_Div1);
        RCC_PCLK2Config(RCC_HCLK_Div1);
        RCC_PCLK1Config(RCC_HCLK_Div2);
        
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
        RCC_PLLCmd(ENABLE);
        
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
        
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        while(RCC_GetSYSCLKSource() != 0x08);
    }
}

/**
 * @brief Init EXTI for PB3/4
 * 
 * @note Mode pull up for GPIOB Pin3/4, button match with GND, event falling
 * 
 * @retval None
 */
void Init_EXTI(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitTypeDef gpiob_pin34_config = {
        .GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4,
        .GPIO_Mode = GPIO_Mode_IPU
    };
    GPIO_Init(GPIOB, &gpiob_pin34_config);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource3);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource4);
       
    EXTI_ClearITPendingBit(EXTI_Line3);
    EXTI_ClearITPendingBit(EXTI_Line4);
    EXTI_InitTypeDef exti_config = {
        .EXTI_Line = EXTI_Line3 | EXTI_Line4,
        .EXTI_Mode = EXTI_Mode_Interrupt,
        .EXTI_Trigger = EXTI_Trigger_Falling,
        .EXTI_LineCmd = ENABLE
    };
    EXTI_Init(&exti_config);

    NVIC_InitTypeDef nvic_config = {
        .NVIC_IRQChannel = EXTI3_IRQn,
        .NVIC_IRQChannelPreemptionPriority = 0,
        .NVIC_IRQChannelSubPriority = 0,
        .NVIC_IRQChannelCmd = ENABLE
    };
    NVIC_Init(&nvic_config);
    nvic_config.NVIC_IRQChannel = EXTI4_IRQn;
    NVIC_Init(&nvic_config);
}

/**
 * @brief GPIOC led Pc13 for debug
 */
void Init_LedPc13(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitTypeDef gpioc_pin13_config = {
        .GPIO_Pin = GPIO_Pin_13,
        .GPIO_Speed = GPIO_Speed_50MHz,
        .GPIO_Mode = GPIO_Mode_Out_PP
    };

    GPIO_Init(GPIOC, &gpioc_pin13_config);
}
