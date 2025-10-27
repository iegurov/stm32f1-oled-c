#include "stm32f1_i2c.h"

#define SDA (Pin){GPIOB, 7}
#define SCL (Pin){GPIOB, 6}

static uint8_t _APB1_presc()
{
    uint8_t raw_APB_presc = (RCC->CFGR >> 8) & 0x7;

    switch (raw_APB_presc)
    {
        case 0x4: return 2; //0b100
        case 0x5: return 4; //0b101
        case 0x6: return 8; //0b110
        case 0x7: return 16;//0b111
        default:  return 1;
    }
}

//Генерация сигнала старт и установка контакта с нужным слейвом
void i2c_start(uint8_t adr, uint8_t dir)
{
    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB)); //Ожидаем флаг StartBit (SB)

    //Сброс SB состоит из чтения SR1(?) и записи/чтения DR
    (void)I2C1->SR1; 

    //Передаем в шину адрес ведомого, с которым хотим обмениваться данными, и режим (запись/чтение)
    I2C1->DR = (adr<<1) | dir; 

    while (!(I2C1->SR1 & I2C_SR1_ADDR)); //Ждем окончания передачи адреса 

    //Сброс ADDR происходит чтением SR1 и SR2
    (void) I2C1->SR1;
    (void) I2C1->SR2;
}

void i2c_stop()
{
    I2C1->CR1 |= I2C_CR1_STOP;
    for(int i = 0; i<5000; i++) __NOP(); //После передачи нужно немного подождать (ЗАМЕНИТЬ НА delayms)
}

void i2c_init()
{
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    PinMode(SDA, AF_OD_50MHz);
    PinMode(SCL, AF_OD_50MHz);

    I2C1->CR1 |= I2C_CR1_SWRST;
    I2C1->CR1 &= ~I2C_CR1_SWRST;

    uint32_t apb1_clock = SystemCoreClock/_APB1_presc();
    uint8_t mhz_clock = (uint8_t)(apb1_clock/1000000); //переводим частоту apb1 в МГц

    //Записываем в первые 5 бит CR2 частоту шины, на которой висит I2C (см. RM0008, стр 774)
    I2C1->CR2 |= mhz_clock & 0x3F;

    //Устанавливаем делитель для получения f_scl = 100кГц (CCR = apb1_clock/(100.000*2))
    I2C1->CCR = apb1_clock/200000;

    //Задаем количество тактов для установления сигнала (корректный сигнал нарастает относительно медленно)
    I2C1->TRISE = mhz_clock+1;

    I2C1->CR1 |= I2C_CR1_PE;
}

void i2c_write(const uint8_t* data, uint16_t len)
{   
    for(uint16_t i = 0; i<len; i++)
    {
        while (!(I2C1->SR1 & I2C_SR1_TXE));
        I2C1->DR = data[i];
    }
    while (!(I2C1->SR1 & I2C_SR1_BTF)); //Ждем окончания передачи байта
}

void i2c_writeByte(uint8_t data)
{        
    I2C1->DR = data;
    while (!(I2C1->SR1 & I2C_SR1_BTF)); //Ждем окончания передачи данных  
}

uint8_t i2c_receiveByte(uint8_t adr)
{
    i2c_start(adr, I2C_R);

    while (!(I2C1->SR1 & I2C_SR1_RXNE)); //Ждем окончания приема данных

    I2C1->CR1 |= I2C_CR1_STOP;

    return I2C1->DR;
}
