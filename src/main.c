//C example

#include "stm32f1_gpio.h"
#include "stm32f1_oled.h"
#include "stm32f1_i2c.h"

int main()
{
    i2c_init();   
    oled_init(SSD1306_128x64);
    oled_fill(0x00);

    oled_px(62, 30, 1);
    oled_px(68, 30, 1);
    oled_px(62, 38, 1);
    oled_px(63, 38, 1);
    oled_px(64, 38, 1);
    oled_px(65, 38, 1);
    oled_px(66, 38, 1);
    oled_px(67, 38, 1);
    oled_px(68, 38, 1);

    oled_update();

    while(1){};
    
    return 0;
}