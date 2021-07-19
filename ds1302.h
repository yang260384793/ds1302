
#ifndef __DS1302_H
#define __DS1302_H

#include "drv_ds1302.h"
#include "driverlib.h"
#include "sys.h"


/*! Define software SPI pins */
#define SPI_CE_PORT
#define SPI_SDA_PIN
#define SPI_SCL_PIN
#define SPI_CE_PORT
#define SPI_CE_PIN

#define SPI_SDA_LOW()
#define SPI_SDA_HIGH()

#define SPI_SCL_LOW()
#define SPI_SCL_HIGH()

#define SPI_CE_LOW()
#define SPI_CE_HIGH()

#define SPI_SDA_READ()

#define SPI_SDA_DIR_INPUT()
#define SPI_SDA_DIR_OUTPUT()

/*! Define software SPI delay */
#define SPI_DELAY(x)           // 此处放置延时函数接口

ds_dev_ctx_t *DS1302_Init(void);

#endif
