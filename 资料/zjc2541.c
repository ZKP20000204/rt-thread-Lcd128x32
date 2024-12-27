
#include "zjc2541.h"
#include "drv_spi.h"
#include <rtthread.h>
#include <rtdevice.h>

int zjc2541_init()
{		
		struct rt_spi_device *spi_dev_zjc2541;
    rt_hw_spi_device_attach(ZJC2541_SPI_BUS, ZJ2541_SPI_DEVICE_NAME, GPIOD, GPIO_PIN_8);
	  
		spi_dev_zjc2541 = (struct rt_spi_device *)rt_device_find(ZJ2541_SPI_DEVICE_NAME);
    if (!spi_dev_zjc2541)
    {
        rt_kprintf("spi sample run failed! can't find %s device!\n", ZJ2541_SPI_DEVICE_NAME);
				return -1;
    }
		
		struct rt_spi_configuration cfg;
		cfg.data_width = 14;
		cfg.mode = RT_SPI_MODE_3 | RT_SPI_MSB;
		cfg.max_hz = 1 * 1000 * 1000; /* 1M */
		rt_spi_configure(spi_dev_zjc2541, &cfg);
		
		return 0;
}

void zjc2541_output(uint16_t data)
{
	struct rt_spi_device *spi_dev_zjc2541;
	spi_dev_zjc2541 = (struct rt_spi_device *)rt_device_find(ZJ2541_SPI_DEVICE_NAME);
	if (!spi_dev_zjc2541)
	{
		rt_kprintf("spi sample run failed! can't find %s device!\n", ZJ2541_SPI_DEVICE_NAME);
		return;
	}
	else
	{        
			rt_spi_transfer(spi_dev_zjc2541, &data, NULL, 1);
	}	
}
