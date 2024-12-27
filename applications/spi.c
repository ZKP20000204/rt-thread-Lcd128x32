#include "spi.h"
#include "drv_spi.h"
#include <rtthread.h>
#include <rtdevice.h>

int SPI_init()
{		
	struct rt_spi_device *lcd_spi_device;
    lcd_spi_device = rt_malloc(sizeof(struct rt_spi_device));
    if (lcd_spi_device == RT_NULL)
    {
        rt_kprintf("Failed to allocate memory for SPI device\n");
        return -1;
    }

    // 挂载 SPI 设备
    rt_hw_spi_device_attach(SPI_BUS, SPI_DEVICE_NAME, GPIOB, GPIO_PIN_12);
    
    // 查找 SPI 设备，确保设备存在
    lcd_spi_device = (struct rt_spi_device *)rt_device_find("spi20");
    if (!lcd_spi_device)
    {
        rt_kprintf("spi sample run failed! can't find device!\n");
        rt_free(lcd_spi_device);
        return -1;
    }

    // 配置 SPI 参数
    struct rt_spi_configuration cfg;
    cfg.data_width = 8;   
    cfg.mode = RT_SPI_MASTER | RT_SPI_MODE_0| RT_SPI_MSB;       
    cfg.max_hz = 10 * 1000 * 1000; // 1 MHz

    if (rt_spi_configure(lcd_spi_device, &cfg) != RT_EOK)
    {
        rt_kprintf("Failed to configure SPI device\n");
        return -1;
    }
    rt_kprintf("SPI1 device initialized successfully\n");
    return 0;
}

void SPI_output(uint16_t data)
{
	struct rt_spi_device *spi_device;
	spi_device = (struct rt_spi_device *)rt_device_find(SPI_DEVICE_NAME);
	if (!spi_device)
	{
		rt_kprintf("spi sample run failed! can't find %s device!\n", SPI_DEVICE_NAME);
		return;
	}
	else
	{        
			rt_spi_transfer(spi_device, &data, NULL, 1);
	}	
}
