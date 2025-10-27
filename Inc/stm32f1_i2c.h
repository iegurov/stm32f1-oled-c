#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1_gpio.h"
#include "stm32f1xx.h"

//Режимы шины
#define I2C_SM 0
#define I2C_FM 1

//Запись/чтение
#define I2C_R 1
#define I2C_W 0


void i2c_init();
void i2c_start(uint8_t adr, uint8_t dir);
void i2c_stop();
void i2c_write(const uint8_t* data, uint16_t len);
void i2c_writeByte(uint8_t data);
uint8_t i2c_read(uint8_t adr);

#ifdef __cplusplus
} //extern "C"
#endif

/*

TODO:
1. Работа с DMA обязательно!!!
2. Режим слейва
*/