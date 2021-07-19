
/*-----------------Include------------------------*/
#include "drv_ds1302.h"

/*-----------------Define------------------------*/
#define HEX_TO_BCD(x)   ((x) % 10 + (x) / 10 * 16)
#define BCD_TO_HEX(x)   ((x) % 16 + (x) / 16 * 10)

#ifndef DRV_BYTE_ORDER
#ifndef __BYTE_ORDER__

#define DRV_LITTLE_ENDIAN 1234
#define DRV_BIG_ENDIAN    4321

/** if _BYTE_ORDER is not defined, choose the endianness of your architecture
  * by uncommenting the define which fits your platform endianness
  */
//#define DRV_BYTE_ORDER    DRV_BIG_ENDIAN
#define DRV_BYTE_ORDER    DRV_LITTLE_ENDIAN

#else /* defined __BYTE_ORDER__ */

#define DRV_LITTLE_ENDIAN  __ORDER_LITTLE_ENDIAN__
#define DRV_BIG_ENDIAN     __ORDER_BIG_ENDIAN__
#define DRV_BYTE_ORDER     __BYTE_ORDER__

#endif /* __BYTE_ORDER__*/
#endif /* DRV_BYTE_ORDER */


/*-----------------Private------------------------*/

/*-----------------Register struct------------------------*/
typedef union
{
    uint8_t value;
    struct
    {
        #if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
        uint8_t Seconds    : 4;
        uint8_t Seconds_10 : 3;
        uint8_t CH         : 1;         /*! 0: 时钟启动 1: 时钟停止 */
        #else
        uint8_t CH         : 1;         /*! 0: 时钟启动 1: 时钟停止 */
        uint8_t Seconds_10 : 3;
        uint8_t Seconds    : 4;
        #endif
    } bits;
} Reg_Seconds_t;

typedef union
{
    uint8_t value;
    struct
    {
        #if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
        uint8_t Minutes    : 4;
        uint8_t Minutes_10 : 3;
        const uint8_t      : 1;
        #else
        const uint8_t      : 1;
        uint8_t Minutes_10 : 3;
        uint8_t Minutes    : 4;
        #endif
    } bits;
} Reg_Minutes_t;

typedef union
{
    uint8_t value;
    struct
    {
        #if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
        uint8_t HourL   : 4;
        uint8_t HourH   : 1;
        uint8_t AM_PM   : 1;  /*! 12小时模式下 0: AM上午 1: PM下午 */
        const uint8_t   : 1;
        uint8_t M_12_24 : 1; /*! 小时模式位,0: 24小时 1: 12小时 */
        #else
        uint8_t M_12_24 : 1; /*! 小时模式位,0: 24小时 1: 12小时 */
        const uint8_t   : 1;
        uint8_t AM_PM   : 1;  /*! 12小时模式下 0: AM上午 1: PM下午 */
        uint8_t HourH   : 1;
        uint8_t HourL   : 4;
        #endif
    } bits;
} Reg_Hours_t;

typedef union
{
    uint8_t value;
    struct
    {
        #if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
        uint8_t Date    : 4;
        uint8_t Date_10 : 2;
        const uint8_t   : 2;
        #else
        const uint8_t   : 2;
        uint8_t Date_10 : 2;
        uint8_t Date    : 4;
        #endif
    } bits;
} Reg_Date_t;

typedef union
{
    uint8_t value;
    struct
    {
        #if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
        uint8_t Month    : 4;
        uint8_t Month_10 : 1;
        const uint8_t    : 3;
        #else
        const uint8_t    : 3;
        uint8_t Month_10 : 1;
        uint8_t Month    : 4;
        #endif
    } bits;
} Reg_Month_t;

typedef union
{
    uint8_t value;
    struct
    {
        #if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
        uint8_t Day   : 3;
        const uint8_t : 5;
        #else
        const uint8_t : 5;
        uint8_t Day   : 3;
        #endif
    } bits;
} Reg_Day_t;

typedef union
{
    uint8_t value;
    struct
    {
        #if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
        uint8_t Year    : 4;
        uint8_t Year_10 : 4;
        #else
        uint8_t Year_10 : 4;
        uint8_t Year    : 4;
        #endif
    } bits;
} Reg_Year_t;

typedef union
{
    uint8_t value;
    struct
    {
        #if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
        const uint8_t : 7;
        uint8_t WP    : 1;    /*! 写保护位 0:关闭保护 1:打开保护 */
        #else
        uint8_t WP    : 1;    /*! 写保护位 0:关闭保护 1:打开保护 */
        const uint8_t : 7;
        #endif
    } bits;
} Reg_Enable_t;

typedef union
{
    uint8_t value;
    struct
    {
        #if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
        uint8_t RS  : 2;
        uint8_t DS  : 2;
        uint8_t TCS : 4;
        #else
        uint8_t TCS : 4;
        uint8_t DS  : 2;
        uint8_t RS  : 2;
        #endif
    } bits;
} Reg_Trickle_t;

/**
 * @name   DS1302_Clock_BurstRead
 * @brief  突发方式连续读取DS1302时钟数据
 * @param  p_buf: 数据缓冲区
 * @return none
 */
static void DS1302_Clock_BurstRead(ds_dev_ctx_t *ctx, uint8_t *p_buf)
{
    ctx->read_reg(REG_CLOCK_BURST_READ, p_buf, 8);
}

/**
 * @name   DS1302_Clock_BurstWrite
 * @brief  突发方式连续写数据到DS1302时钟
 * @param  p_buf: 数据缓冲区
 * @return none
 */
static void DS1302_Clock_BurstWrite(ds_dev_ctx_t *ctx, uint8_t *p_buf)
{
    ctx->write_reg(REG_CLOCK_BURST_READ, p_buf, 8);
}

/**
 * @name   DS1302_EnableWP
 * @brief  Enable DS1302 write protection
 * @param  none
 * @return none
 */
static void DS1302_EnableWP(ds_dev_ctx_t *ctx)
{
    Reg_Enable_t reg;
    
    reg.bits.WP = 1;
    ctx->write_reg(REG_ENABLE, &reg.value, 1);
}

/**
 * @name   DS1302_DisableWP
 * @brief  Disable DS1302 write protection
 * @param  none
 * @return none
 */
static void DS1302_DisableWP(ds_dev_ctx_t *ctx)
{
    Reg_Enable_t reg;
    
    reg.bits.WP = 0;
    ctx->write_reg(REG_ENABLE, &reg.value, 1);
}

/**
 * @name   DS1302_ClockEnable
 * @brief  Eanble clock，DS1302 start work
 * @param  none
 * @return none
 */
void DS1302_ClockEnable(ds_dev_ctx_t *ctx)
{
    Reg_Seconds_t reg;

	/*! Get current register value */
    ctx->read_reg(REG_SECONDS, &reg.value, 1);
    reg.bits.CH = 0;

	/*! Update data to secdons register */
    DS1302_DisableWP(ctx);
    ctx->write_reg(REG_SECONDS, &reg.value, 1);
    DS1302_EnableWP(ctx);
}

/**
 * @name   DS1302_ClockDisable
 * @brief  Disable clock，DS1302 stop work
 * @param  none
 * @return none
 */
void DS1302_ClockDisable(ds_dev_ctx_t *ctx)
{
    Reg_Seconds_t reg;

	/*! Get current register value */
    ctx->read_reg(REG_SECONDS, &reg.value, 1);
    reg.bits.CH = 1;
	
	/*! Update data to seconds register */
    DS1302_DisableWP(ctx);
    ctx->write_reg(REG_SECONDS, &reg.value, 1);
	DS1302_EnableWP(ctx);
}

uint8_t DS1302_Get_Clock_Run(ds_dev_ctx_t *ctx)
{
    Reg_Seconds_t reg;

	/*! Get current register value */
    ctx->read_reg(REG_SECONDS, &reg.value, 1);
    if (reg.bits.CH == 1)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/**
 * @name   DS1302_Set_TimeFormat
 * @brief  Setting DS1302 clock 12Hour/24Hour format
 * @param  mode: FORMAT_24_HOUR/FORMAT_12_HOUR
 * @return none
 */
void DS1302_Set_TimeFormat(ds_dev_ctx_t *ctx, TimeFormat_t fmt)
{
    Reg_Hours_t reg;

	/*! Get current register value */
    ctx->read_reg(REG_HOURS, &reg.value, 1);
    if (fmt == FORMAT_24_HOUR)
    {
    	/*! Setting hour register high bit 0，enable 24 hour format */
        reg.bits.M_12_24 = 0;
    }
    else
    {
    	/*! Setting hour register high bit 1，enable 12 hour format */
        reg.bits.M_12_24 = 1;
    }
	/*! Update data to hour register */
    DS1302_DisableWP(ctx);
    ctx->write_reg(REG_HOURS, &reg.value, 1);
    DS1302_EnableWP(ctx);
}

/**
 * @name   DS1302_Get_TimeFormat
 * @brief  Get DS1302 current time format
 * @param  none
 * @return FORMAT_24_HOUR
 * #return FORMAT_12_HOUR
 */
TimeFormat_t DS1302_Get_TimeFormat(ds_dev_ctx_t *ctx)
{
    Reg_Hours_t reg;

	/*! Get current register value */
    ctx->read_reg(REG_HOURS, &reg.value, 1);
    if (reg.bits.M_12_24 == 0)
    {
        return FORMAT_24_HOUR;
    }
    else
    {
        return FORMAT_12_HOUR;
    }
}

/**
 * @name   DS1302_EnableChangle
 * @brief  开启DS1302 慢速充电功能
 * @param  diode: 选择降压二极管
 *                -DIODE_0_7
 *                -DIODE_1_4
 * @param  res:   选择限流电阻
 *                -RES_NONE
 *                -RES_2K
 *                -RES_4K
 *                -RES_8K
 * @return none
 */
void DS1302_EnableCharge(ds_dev_ctx_t *ctx, Diode_t diode, Res_t res)
{
    Reg_Trickle_t Trickle;

	/*! Get current register value */
    ctx->read_reg(REG_TRICKLE, &Trickle.value, 1);
    Trickle.bits.TCS = 0x0A;
    Trickle.bits.DS = diode;
    Trickle.bits.RS = res;

	/*! Update data to trickle register */
    DS1302_DisableWP(ctx);
    ctx->write_reg(REG_TRICKLE, &Trickle.value, 1);
    DS1302_EnableWP(ctx);
}

/**
 * @name   DS1302_DisableChangle
 * @brief  关闭DS1302 慢速充电功能
 * @param  none
 * @return none
 */
void DS1302_DisableCharge(ds_dev_ctx_t *ctx)
{
    Reg_Trickle_t Trickle;

	/*! Get current register value */
    ctx->read_reg(REG_TRICKLE, &Trickle.value, 1);
    Trickle.bits.TCS = 0x00;
    Trickle.bits.DS = 0;
    Trickle.bits.RS = 0;

	/*! Update data to trickle register */
    DS1302_DisableWP(ctx);
    ctx->write_reg(REG_TRICKLE, &Trickle.value, 1);
    DS1302_EnableWP(ctx);
}

/**
 * @name   DS1302_GetTime
 * @brief  获取DS1302 当前时间
 * @param  ds_time: RTC时间结构体指针
 * @return none
 */
void DS1302_GetTime(ds_dev_ctx_t *ctx, DS_Time_t * ds_time)
{
    Reg_Seconds_t Sec_Reg;
    Reg_Minutes_t Min_Reg;
    Reg_Hours_t Hour_Reg;

    ctx->read_reg(REG_SECONDS, &Sec_Reg.value, 1);
    ctx->read_reg(REG_MINUTES, &Min_Reg.value, 1);
    ctx->read_reg(REG_HOURS, &Hour_Reg.value, 1);

    if (Hour_Reg.bits.M_12_24 == 0)
    {
        ds_time->hour = BCD_TO_HEX(Hour_Reg.value & 0x3F);
    }
    else
    {
        ds_time->am_pm = Hour_Reg.bits.AM_PM;
        ds_time->hour = BCD_TO_HEX(Hour_Reg.value & 0x1F);
    }
    ds_time->min = BCD_TO_HEX(Min_Reg.value);
    ds_time->sec = BCD_TO_HEX(Sec_Reg.value & 0x7F);
}

/**
 * @name   DS1302_GetDate
 * @brief  获取DS1302当前日期
 * @param  ds_date: RTC日期结构体指针
 * @return none
 */
void DS1302_GetDate(ds_dev_ctx_t *ctx, DS_Date_t * ds_date)
{
    Reg_Date_t Date_Reg;
    Reg_Month_t Month_Reg;
    Reg_Day_t Day_Reg;
    Reg_Year_t Year_Reg;

    ctx->read_reg(REG_DATE, &Date_Reg.value, 1);
    ctx->read_reg(REG_MONTH, &Month_Reg.value, 1);
    ctx->read_reg(REG_DAY, &Day_Reg.value, 1);
    ctx->read_reg(REG_YEAR, &Year_Reg.value, 1);
    
    ds_date->year = BCD_TO_HEX(Year_Reg.value);
    ds_date->month = BCD_TO_HEX(Month_Reg.value & 0x1F);
    ds_date->date = BCD_TO_HEX(Date_Reg.value & 0x3F);
    ds_date->day = BCD_TO_HEX(Day_Reg.value & 0x07);
}

/**
 * @name   DS1302_SetTime
 * @brief  设置DS1302 时间
 * @param  ds_time: RTC时间结构体指针
 * @return none
 */
void DS1302_SetTime(ds_dev_ctx_t *ctx, DS_Time_t * ds_time)
{
    Reg_Seconds_t Sec_Reg;
    Reg_Minutes_t Min_Reg;
    Reg_Hours_t Hour_Reg;

    ctx->read_reg(REG_SECONDS, &Sec_Reg.value, 1);
    ctx->read_reg(REG_MINUTES, &Min_Reg.value, 1);
    ctx->read_reg(REG_HOURS, &Hour_Reg.value, 1);
    
    DS1302_DisableWP(ctx); /*! 关闭写保护 */
    Sec_Reg.value &= 0x80;
    if (Hour_Reg.bits.M_12_24 == 0)
    {
        Hour_Reg.value &= 0x80;
    }
    else
    {
        Hour_Reg.value &= 0xA0;
        Hour_Reg.bits.AM_PM = ds_time->am_pm;
    }

    Sec_Reg.value |= HEX_TO_BCD(ds_time->sec);
    Min_Reg.value = HEX_TO_BCD(ds_time->min);
    Hour_Reg.value |= HEX_TO_BCD(ds_time->hour);
    
    ctx->write_reg(REG_SECONDS, &Sec_Reg.value, 1);
    ctx->write_reg(REG_MINUTES, &Min_Reg.value, 1);
    ctx->write_reg(REG_HOURS, &Hour_Reg.value, 1);
    DS1302_EnableWP(ctx); /*! 打开写保护 */
}

/**
 * @name   DS1302_SetDate
 * @brief  设置DS1302 日期
 * @param  ds_date: RTC日期结构体指针
 * @return none
 */
void DS1302_SetDate(ds_dev_ctx_t *ctx, DS_Date_t * ds_date)
{
    DS_Date_t temp_date;
    
    DS1302_DisableWP(ctx);                 /*! 关闭写保护 */
    temp_date.year = HEX_TO_BCD(ds_date->year);
    temp_date.day = HEX_TO_BCD(ds_date->day);
    temp_date.month = HEX_TO_BCD(ds_date->month);
    temp_date.date = HEX_TO_BCD(ds_date->date);

    ctx->write_reg(REG_DATE, &temp_date.date, 1);
    ctx->write_reg(REG_MONTH, &temp_date.month, 1);
    ctx->write_reg(REG_DAY, &temp_date.day, 1);
    ctx->write_reg(REG_YEAR, &temp_date.year, 1);
    DS1302_EnableWP(ctx);                  /*! 打开写保护 */
}

/**
 * @name   DS1302_GetTime
 * @brief  获取DS1302当前时间与日期
 * @param  td_stc: RTC时间与日期结构体指针
 * @return none
 */
void DS1302_Get_TimeDate(ds_dev_ctx_t *ctx, DS_TimeDate_t *td_stc)
{
    uint8_t TimeDate[8] = {0};

    DS1302_Clock_BurstRead(ctx, TimeDate);
    td_stc->Time.sec = BCD_TO_HEX(TimeDate[0] & 0x7F);
    td_stc->Time.min = BCD_TO_HEX(TimeDate[1]);
    if (!(TimeDate[2] & 0x80))
    {
        td_stc->Time.hour = BCD_TO_HEX(TimeDate[2] & 0x3F);
    }
    else
    {
        td_stc->Time.am_pm = TimeDate[2] & 0x20;
        td_stc->Time.hour = BCD_TO_HEX(TimeDate[2] & 0x1F);
    }
    
    td_stc->Date.date = BCD_TO_HEX(TimeDate[3] & 0x3F);
    td_stc->Date.month = BCD_TO_HEX(TimeDate[4] & 0x1F);
    td_stc->Date.day = BCD_TO_HEX(TimeDate[5] & 0x07);
    td_stc->Date.year = BCD_TO_HEX(TimeDate[6]);
}

/**
 * @name   DS1302_SetDate
 * @brief  设置DS1302 时间与日期
 * @param  td_stc: RTC时间与日期结构体指针
 * @return none
 */
void DS1302_Set_TimeDate(ds_dev_ctx_t *ctx, DS_TimeDate_t *td_stc)
{
    uint8_t TimeDate[8] = {0};

    ctx->read_reg(REG_SECONDS, &TimeDate[0], 1);
    ctx->read_reg(REG_HOURS, &TimeDate[2], 1);

    TimeDate[0] &= 0x80;
    if (!(TimeDate[2] & 0x80))
    {
        TimeDate[2] &= 0x80;
    }
    else
    {
        TimeDate[2] &= 0xA0;
        TimeDate[2] |= td_stc->Time.am_pm;
    }
    TimeDate[0] |= HEX_TO_BCD(td_stc->Time.sec);
    TimeDate[1] = HEX_TO_BCD(td_stc->Time.min);
    TimeDate[2] |= HEX_TO_BCD(td_stc->Time.hour);
    TimeDate[3] = HEX_TO_BCD(td_stc->Date.date);
    TimeDate[4] = HEX_TO_BCD(td_stc->Date.month);
    TimeDate[5] = HEX_TO_BCD(td_stc->Date.day);
    TimeDate[6] = HEX_TO_BCD(td_stc->Date.year);
    TimeDate[7] = 0x80;
    
    DS1302_DisableWP(ctx);
    DS1302_Clock_BurstWrite(ctx, TimeDate);
    DS1302_EnableWP(ctx);
}
