#include "bdang_i2c1.h"

void init_i2c1(void){
    I2C_InitTypeDef i2c1_config = {
        .I2C_ClockSpeed = 100000,
        .I2C_Mode = I2C_Mode_I2C,
        .I2C_DutyCycle = I2C_DutyCycle_2,
        .I2C_OwnAddress1 = 0x00,
        .I2C_Ack = I2C_Ack_Enable, 
        .I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit
    };

    I2C_Init(I2C1, &i2c1_config);
}