#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_flash.h"
#include "bdang_systick.h"
#include "w25q32.h"
#include "spi1.h"

/* Declaration zone ------------------------------------------------------ */
void SystemClock_Config(void);
void Init_LedPc13(void);
void Test_W25Q32_WriteRead(void);


/* Global variables ------------------------------------------------------ */
volatile uint32_t msTicks = 0;


/* MAIN function --------------------------------------------------------------- */
int main(void) {
    SystemClock_Config();
    systick_init(1000);
    SPI1_Init();
    Init_LedPc13();

    // uint8_t manu;
    // uint16_t id;
    // W25Q32_ReadID(&manu, &id);

    Test_W25Q32_WriteRead();
    while(1) {
        GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
        delay_ms(500);
        GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
        delay_ms(500);
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

void Test_W25Q32_WriteRead(void) {
    uint32_t test_address = 0x000000;  
    
    W25Q32_EraseSector(test_address);
    delay_ms(500); 
    
    uint8_t write_data[20] = "HELLO WORLD STM32!"; 
    uint8_t read_data[20] = {0};  
    
    W25Q32_WritePage(test_address, write_data, 19);
    delay_ms(10); 
    
    W25Q32_ReadData(test_address, read_data, 19);
}