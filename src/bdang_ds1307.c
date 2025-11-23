#include "bdang_ds1307.h"

void ds1307_start(void){
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
}

void ds1307_send_addr(uint8_t addr, uint8_t direction){
    I2C_Send7bitAddress(I2C1, addr, direction);
    
    if(direction == I2C_Direction_Transmitter){
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    } else {
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    }
}

void ds1307_stop(void){
    I2C_GenerateSTOP(I2C1, ENABLE);
}

void ds1307_send_data(uint8_t data){
    I2C_SendData(I2C1, data);  
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}

uint8_t ds1307_read(uint8_t ack){
    if (ack){
        I2C_AcknowledgeConfig(I2C1, ENABLE);
    }
    else{
        I2C_AcknowledgeConfig(I2C1, DISABLE);
    }
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
    uint8_t data = I2C_ReceiveData(I2C1);
    return data;
}

void ds1307_gettime(uint8_t* time){
    ds1307_start();
    ds1307_send_addr(DS1307_ADDR << 1, I2C_Direction_Transmitter);
    ds1307_send_data(0x00);  
    
    ds1307_start();
    ds1307_send_addr(DS1307_ADDR << 1, I2C_Direction_Receiver);
    
    for (int i = 0; i < 6; i++){
        time[i] = ds1307_read(1);  
    }
    time[6] = ds1307_read(0);  
    ds1307_stop();
}

void ds1307_enable(void){
    uint8_t sec;
    
    ds1307_start();
    ds1307_send_addr(DS1307_ADDR << 1, I2C_Direction_Transmitter);
    ds1307_send_data(0x00); 
    
    ds1307_start(); 
    ds1307_send_addr(DS1307_ADDR << 1, I2C_Direction_Receiver);
    sec = ds1307_read(0); 
    ds1307_stop();
    
    sec &= ~(1 << 7);
    
    ds1307_start();
    ds1307_send_addr(DS1307_ADDR << 1, I2C_Direction_Transmitter);
    ds1307_send_data(0x00);  
    ds1307_send_data(sec);  
    ds1307_stop();
}

uint8_t bcd2dec(uint8_t val){
    return ((val >> 4) * 10) + (val & 0x0F);
}

uint8_t dec2bcd(uint8_t val) {
    return ((val / 10) << 4) | (val % 10);
}