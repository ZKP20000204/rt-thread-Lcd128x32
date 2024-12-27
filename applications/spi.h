#ifndef __SPI_H
#define __SPI_H

#include "n32g45x.h"
#include <rtthread.h>
#include "drv_spi.h"

#define SPI_BUS			  "spi2"
#define SPI_DEVICE_NAME    "spi20"

int SPI_init(void);
void SPI_output(uint16_t data);

#endif

