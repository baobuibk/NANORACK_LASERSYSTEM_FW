/*
 * measurement.h
 *
 * Created: 5/28/2024 4:26:05 AM
 *  Author: DELL
 */ 


#ifndef MEASUREMENT_H_
#define MEASUREMENT_H_
#include "scheduler.h"
#include "Laserboard.h"
#include "uart.h"
#include "AD.h"
#include "monitor.h"
#include "i2c.h"

typedef struct _Measurement_Typedef_
{
	uint16_t current_internal;
	uint16_t current_external;
}Measurement_Typedef;

extern Measurement_Typedef s_Measurement;
void measure_init(void);
void measure_create_task(void);

#endif /* MEASUREMENT_H_ */