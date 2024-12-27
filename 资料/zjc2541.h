#ifndef __ZJC2541_H
#define __ZJC2541_H

#include "n32g45x.h"
#include <rtthread.h>
#include "drv_spi.h"

#define ZJC2541_SPI_BUS						"spi2"
#define ZJ2541_SPI_DEVICE_NAME    "spi20"

int zjc2541_init(void);
void zjc2541_output(uint16_t data);

#endif

