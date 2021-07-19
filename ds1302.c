
#include "ds1302.h"


static ds_dev_ctx_t dev_ctx;
static TimeFormat_t CurTimeFormat;

static DS_Time_t Defalut_Time = 
{
    .hour = 12,
    .min  = 32,
    .sec  = 00,
};

static DS_Date_t Defalut_Date = 
{
    .year  = 21,
    .month = 4,
    .date  = 29,
    .day   = 4,
};

static DS_TimeDate_t Defalut_TimeDate = 
{
    .Time.hour  = 12,
    .Time.min   = 32,
    .Time.sec   = 00,
    .Date.year  = 21,
    .Date.month = 4,
    .Date.date  = 29,
    .Date.day   = 4,
};

/**
 * @name   SPI_WriteByte
 * @brief  SPI写一字节数据
 * @param  byte
 * @return 
 */
static void SPI_WriteByte(uint8_t byte)
{
    uint8_t i;

    for (i = 0; i < 8; i++)
    {
        SPI_SCL_LOW();
        if (byte & 0x01)
            SPI_SDA_HIGH();
        else
            SPI_SDA_LOW();
        SPI_SCL_HIGH();
        SPI_DELAY(1);
        byte >>= 1;
        SPI_DELAY(1);
    }
    SPI_SCL_LOW();
    SPI_SDA_HIGH();
}

/**
 * @name   SPI_ReadByte
 * @brief  SPI读一字节数据
 * @param  byte
 * @return 
 */
static uint8_t SPI_ReadByte(void)
{
    uint8_t i;
    uint8_t byte = 0;

    SPI_SDA_DIR_INPUT();
    for (i = 0; i < 8; i++)
    {
        byte >>= 1;
        if (SPI_SDA_READ())
            byte |= 0x80;
        SPI_SCL_HIGH();
        SPI_DELAY(1);
        SPI_SCL_LOW();
        SPI_DELAY(1);
    }
    SPI_SDA_DIR_OUTPUT();
    SPI_SDA_HIGH();
    return byte;
}

/**
 * @name   DS1302_WriteRegister
 * @brief  写数据到DS1302寄存器
 * @param  reg:  寄存器地址
 * @param  data: 写入到寄存器的数据
 * @return none
 */
static void DS1302_WriteRegister(uint8_t reg, uint8_t *p_buf, uint16_t len)
{
    uint16_t i;
    
    SPI_CE_HIGH();
    SPI_WriteByte(reg);
    SPI_DELAY(1);
    for (i = 0; i < len; i++)
    {
        SPI_WriteByte(p_buf[i]);
        SPI_DELAY(1);
    }
    SPI_CE_LOW();
}

/**
 * @name   DS1302_ReadRegister
 * @brief  从DS1302寄存器读数据
 * @param  reg:  寄存器地址
 * @return none
 */
static void DS1302_ReadRegister(uint8_t reg, uint8_t *p_buf, uint16_t len)
{
    uint8_t i;

    SPI_CE_HIGH();
    SPI_WriteByte(reg | 0x01);
    SPI_DELAY(1);
    for (i = 0; i < len; i++)
    {
        p_buf[i] = SPI_ReadByte();
        SPI_DELAY(1);
    }
    SPI_CE_LOW();
}

/**
 * @name   SPI_GPIO_Init
 * @brief  初始化DS1302 SPI接口
 * @param  none
 * @return none
 */
static void SPI_GPIO_Init(void)
{
    
}

/**
 * @name   DS1302_Init
 * @brief  初始化DS1302配置
 * @param  none
 * @return none
 */
ds_dev_ctx_t *DS1302_Init(void)
{
    dev_ctx.read_reg = DS1302_ReadRegister;
    dev_ctx.write_reg = DS1302_WriteRegister;
    SPI_GPIO_Init();

    CurTimeFormat = DS1302_Get_TimeFormat(&dev_ctx);
    /*! 判断秒寄存器CH位为1则上次掉电 */
    if (!DS1302_Get_Clock_Run(&dev_ctx))
    {
        /*! 设置默认时间和日期 */
        DS1302_Set_TimeFormat(&dev_ctx, FORMAT_24_HOUR);
        /*! 设置24小时格式 */
        DS1302_Set_TimeDate(&dev_ctx, &Defalut_TimeDate);
        /*! 使能时钟 */
        DS1302_ClockEnable(&dev_ctx);
    }
    return &dev_ctx;
}
