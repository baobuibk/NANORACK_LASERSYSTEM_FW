/*
 * measurement.c
 *
 * Created: 5/28/2024 4:26:27 AM
 *  Author: DELL
 */ 

#include "measurement.h"

static void measure_task_update(void);
Measurement_Typedef s_Measurement;

typedef struct _Measure_TaskContextTypedef_
{
	SCH_TASK_HANDLE               taskHandle;
	SCH_TaskPropertyTypedef       taskProperty;
} Measure_TaskContextTypedef;


static Measure_TaskContextTypedef           s_measure_task_context =
{
	SCH_INVALID_TASK_HANDLE,                 // Will be updated by Schedular
	{
		SCH_TASK_SYNC,                      // taskType;
		SCH_TASK_PRIO_0,                    // taskPriority;
		100,                                // taskPeriodInMS;
		measure_task_update                // taskFunction;
	}
};


void measure_init(void)
{
	ADC_Init();
}



static	void	measure_task_update(void)
{
	s_Measurement.current_internal = get_current_100times(ADC_Data.Data[ADC_CURRENT_INT]);
	s_Measurement.current_external = get_current_100times(ADC_Data.Data[ADC_CURRENT_EXT]);
	
	txbuffer[I2C_CURRENT_INT_ADDR] = (uint8_t)(s_Measurement.current_internal>>8);
	txbuffer[I2C_CURRENT_INT_ADDR+1] = (uint8_t)s_Measurement.current_internal;
	
	txbuffer[I2C_CURRENT_EXT_ADDR] = (uint8_t)(s_Measurement.current_external>>8);
	txbuffer[I2C_CURRENT_EXT_ADDR+1] = (uint8_t)s_Measurement.current_external;
}

void measure_create_task(void)
{
	measure_init();
	SCH_TASK_CreateTask(&s_measure_task_context .taskHandle, &s_measure_task_context .taskProperty);
}
