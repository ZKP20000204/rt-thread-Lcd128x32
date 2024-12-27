/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-19     Nations      first version
 */

#ifndef __DRV_SPI_H__
#define __DRV_SPI_H__

#include <rthw.h>
#include <rtthread.h>
#include <board.h>

#ifdef __cplusplus
extern "C" {
#endif

struct n32_spi_cs
{
    GPIO_Module* GPIOx;
    uint32_t GPIO_Pin;
};

int rt_hw_spi_init(void);
rt_err_t rt_hw_spi_device_attach(const char *bus_name, const char *device_name, GPIO_Module *cs_gpiox, uint32_t cs_gpio_pin);
#ifdef __cplusplus
}
#endif

#endif /* __DRV_SPI_H__ */
