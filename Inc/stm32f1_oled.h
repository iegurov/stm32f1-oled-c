#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1_gpio.h"
#include "stm32f1_i2c.h"

#define SSD1306_128x64 0
#define SSD1306_128x32 1

#define OLED_ADDR 0x3C //или 0x3D

#define OLED_C  0x00 //Команда
#define OLED_D  0x40 //Данные

void oled_init(uint8_t type);
void oled_cmd (uint8_t cmd);
void oled_data(const uint8_t* data, uint16_t len);
void oled_fill(uint8_t fill);
void oled_px(uint8_t x, uint8_t y, uint8_t fill);
void oled_update();

#ifdef __cplusplus
} //extern "C"
#endif

//TODO
//1. Убрать лишние транзакции данных при работе с 128x32 дисплеем
//2. Добавить простейшую геометрию
//3. Добавить вывод текста
//4. Работа с DMA 