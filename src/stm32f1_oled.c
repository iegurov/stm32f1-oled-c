#include "stm32f1_oled.h"
#include "stm32f1_i2c.h"

//Команды инициализации согласно SSD1306 App Note (стр. 5)
const uint8_t init_seq[] = {
    0xAE,       
    0xD5, 0x80,
    0xA8, 0x3F,
    //0xA8, 0x1F, 128x32?
    0xD3, 0x00,
    0x40,
    0x8D, 0x14,
    0x20, 0x00,
    0xA1, 0xC8,
    0xDA, 0x12,
    //0xDA, 0x02, 128x32?
    0x81, 0x7F,
    0xD9, 0xF1,
    0xDB, 0x40,
    0xA4, 0xA6,
    0xAF
   
};

static uint8_t data_buf[128*8] = {0};
static uint8_t maxX, maxY;

void oled_init(uint8_t type)
{
    if(type==SSD1306_128x64)
    {
        maxX = 127;
        maxY = 63;
    }
    else
    {
        maxX = 127;
        maxY = 31;
    }

    i2c_start(OLED_ADDR, I2C_W);
    i2c_writeByte(OLED_C);
    i2c_write(init_seq, sizeof(init_seq));
    i2c_stop();
}

//Отправка массива данных произвольной длины
void oled_data(const uint8_t* data, uint16_t len)
{
    i2c_start(OLED_ADDR, I2C_W);
    
    i2c_writeByte(OLED_D);
    i2c_write(data, len);

    i2c_stop();
}

//Отправка одиночного байта данных (не особо полезно) 
void oled_byte(uint8_t b)
{
    i2c_start(OLED_ADDR, I2C_W);
    i2c_writeByte(OLED_D);
    i2c_writeByte(b);
    i2c_stop();
}

//Отправка команды
void oled_cmd(uint8_t cmd)
{
    i2c_start(OLED_ADDR, I2C_W);
    i2c_writeByte(OLED_C);
    
    i2c_writeByte(cmd);
    i2c_stop(); 
}

//Отправка команды с параметром
void oled_cmd2(uint8_t cmd, uint8_t cmd_arg)
{
    i2c_start(OLED_ADDR, I2C_W);
    i2c_writeByte(OLED_C);
    i2c_writeByte(cmd);
    i2c_writeByte(cmd_arg);
    i2c_stop(); 
}

//Установка рабочей области экрана
void oled_setWindow(uint8_t x0, uint8_t x1, uint8_t y0, uint8_t y1)
{
    i2c_start(OLED_ADDR, I2C_W);

    i2c_writeByte(OLED_C);

    i2c_writeByte(0x21); //col
    i2c_writeByte(x0); //start
    i2c_writeByte(x1); //end

    i2c_writeByte(0x22); //page
    i2c_writeByte(y0); //start
    i2c_writeByte(y1); //end

    i2c_stop();
}

//Обновление экрана
void oled_update()
{
    oled_setWindow(0, maxX, 0, maxY);

    i2c_start(OLED_ADDR, I2C_W);
    i2c_writeByte(OLED_D);

    for(uint16_t i = 0; i<1024; i++) i2c_writeByte(data_buf[i]);
    i2c_stop();
}

void oled_fill(uint8_t fill)
{
    //oled_setWindow(0, 127, 0, 7);

    //i2c_start(OLED_ADDR, I2C_W);
    //i2c_writeByte(OLED_D);

    for(uint16_t i = 0; i<1024; i++) data_buf[i] = fill;
    //i2c_stop();
}

void oled_px(uint8_t x, uint8_t y, uint8_t fill)
{
    //oled_setWindow(x, 127, y>>3, 7);
    //x*((y>>3)+1)
    if(fill==0) 
    {
        data_buf[(y/8)*128+x] &= ~(1<<(y&0b111));
    }
    else 
    {
        data_buf[(y/8)*128+x] |= 1<<(y&0b111);
    }

    //i2c_start(OLED_ADDR, I2C_W);
    //i2c_writeByte(OLED_D);
    //i2c_writeByte(); //y&0b111 - остаток от деления на 8, чтобы получить позицию пикселя в ряде (column)
    //i2c_stop();
}

