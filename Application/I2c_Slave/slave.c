/*
 * slave.c
 *
 * Created: 11/24/2024 11:30:01 PM
 *  Author: DELL
 */ 

#include "slave.h"

static void slave_task_update(void);

typedef struct _Slave_TaskContextTypedef_
{
	SCH_TASK_HANDLE               taskHandle;
	SCH_TaskPropertyTypedef       taskProperty;
} Slave_TaskContextTypedef;


static Slave_TaskContextTypedef           s_slave_task_context =
{
	SCH_INVALID_TASK_HANDLE,                 // Will be updated by Schedular
	{
		SCH_TASK_SYNC,                      // taskType;
		SCH_TASK_PRIO_0,                    // taskPriority;
		50,                                // taskPeriodInMS;
		slave_task_update                // taskFunction;
	}
};


void slave_init(void)
{
	i2c_slave_init(I2C_SLAVE_ADDRESS);
}



static	void	slave_task_update(void)
{
	
}

void slave_create_task(void)
{
	slave_init();
	SCH_TASK_CreateTask(&s_slave_task_context .taskHandle, &s_slave_task_context .taskProperty);
}
