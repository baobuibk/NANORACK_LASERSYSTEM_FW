/*
 * PMUFirmware.c
 *
 * Created: 5/15/2024 10:28:50 AM
 * Author : Admin
 */ 

#include "Laserboard.h"
#include "scheduler.h"
#include "status_led.h"
#include "command.h"
#include "control.h"
#include "measurement.h"
#include "i2c.h"

int main(void)
{
	i2c_slave_init(I2C_SLAVE_ADDRESS);
	SPI_Init();
 	TPL5010_init();
 	SCH_Initialize();
 	status_led_create_task();
 	measure_create_task();
	control_create_task();
 	command_create_task();
	//slave_create_task();
	SCH_StartSchedular();
	
	while (1)
	{
		SCH_HandleScheduledTask();
	}
}
