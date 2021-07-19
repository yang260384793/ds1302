
#ifndef __DRV_DS1302_H
#define __DRV_DS1302_H

#include "stdint.h"

/*! Define dS1302 register map */
#define REG_SECONDS             0x80
#define REG_MINUTES             0x82
#define REG_HOURS               0x84
#define REG_DATE                0x86
#define REG_MONTH               0x88
#define REG_DAY                 0x8A
#define REG_YEAR                0x8C
#define REG_ENABLE              0x8E
#define REG_TRICKLE             0x90
#define REG_CLOCK_BURST         0xBE
#define REG_CLOCK_BURST_WRITE   0xBE
#define REG_CLOCK_BURST_READ    0xBF
#define REG_RAM_START           0xC0
#define REG_RAM_END             0xFC
#define REG_RAM_BURST           0xFE
#define REG_RAM_BURST_WRITE     0xFE
#define REG_RAM_BURST_READ      0xFF

typedef void (*ds_dev_write_ptr)(uint8_t, uint8_t *,
                                    uint16_t);
typedef void (*ds_dev_read_ptr) (uint8_t, uint8_t *,
                                    uint16_t);

typedef struct {
  /** Component mandatory fields **/
  ds_dev_write_ptr  write_reg;
  ds_dev_read_ptr   read_reg;
} ds_dev_ctx_t;

/**
 * enum ds1302 time format
 */ 
typedef enum
{
    FORMAT_24_HOUR = 0,
    FORMAT_12_HOUR,
} TimeFormat_t;

/**
 * enum battery charge diode
 */ 
typedef enum
{
    DIODE_0_7 = 0, /*! One diode has a voltage drop of 0.7V */
    DIODE_1_4 = 1, /*! Two diode has a voltage drop of 1.4V */
} Diode_t;

/**
 * enum battery charge resistance
 */ 
typedef enum
{
    RES_NONE = 0,
    RES_2K,
    RES_4K,
    RES_8K,
} Res_t;

typedef struct
{
    uint8_t am_pm;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
} DS_Time_t;

typedef struct
{
    uint8_t year;
    uint8_t month;
    uint8_t date;
    uint8_t day;
} DS_Date_t;

typedef struct
{
    DS_Time_t Time;
    DS_Date_t Date;
} DS_TimeDate_t;

void DS1302_ClockEnable(ds_dev_ctx_t *ctx);
void DS1302_ClockDisable(ds_dev_ctx_t *ctx);
uint8_t DS1302_Get_Clock_Run(ds_dev_ctx_t *ctx);
void DS1302_Set_TimeFormat(ds_dev_ctx_t *ctx, TimeFormat_t fmt);
TimeFormat_t DS1302_Get_TimeFormat(ds_dev_ctx_t *ctx);
void DS1302_EnableCharge(ds_dev_ctx_t *ctx, Diode_t diode, Res_t res);
void DS1302_DisableCharge(ds_dev_ctx_t *ctx);
void DS1302_GetTime(ds_dev_ctx_t *ctx, DS_Time_t *ds_time);
void DS1302_GetDate(ds_dev_ctx_t *ctx, DS_Date_t *ds_date);
void DS1302_SetTime(ds_dev_ctx_t *ctx, DS_Time_t *ds_time);
void DS1302_SetDate(ds_dev_ctx_t *ctx, DS_Date_t *ds_date);
void DS1302_Get_TimeDate(ds_dev_ctx_t *ctx, DS_TimeDate_t *td_stc);
void DS1302_Set_TimeDate(ds_dev_ctx_t *ctx, DS_TimeDate_t *td_stc);

#endif
