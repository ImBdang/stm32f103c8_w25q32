#ifndef __DS1307__
#define __DS1307__
#define DS1307_ADDR 0x68

#include "stdint.h"
#include "bdang_i2c1.h"

uint8_t dec2bcd(uint8_t val);
uint8_t bcd2dec(uint8_t val);

void ds1307_enable(void);
void ds1307_start(void);
void ds1307_send_addr(uint8_t add, uint8_t direction);
void ds1307_stop(void);
void ds1307_send_data(uint8_t data);
uint8_t ds1307_read(uint8_t ack);
void ds1307_gettime(uint8_t* time);

#endif 