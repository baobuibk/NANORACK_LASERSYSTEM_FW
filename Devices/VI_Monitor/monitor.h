/*
 * monitor.h
 *
 * Created: 5/27/2024 10:30:53 PM
 *  Author: DELL
 */ 


#ifndef MONITOR_H_
#define MONITOR_H_

#include <stdint.h>
#include <math.h>
#include "basetypedef.h"

#define _ADC_MAX		1023
#define _VREF_ADC		5.0f
#define _R_SHUNT		30


float voltage_cal(uint16_t adcValue, uint16_t res_up, uint16_t res_low);
float current_cal(uint16_t adcValue);
uint16_t get_voltage_100times(uint16_t adcValue, uint16_t res_up, uint16_t res_low);
uint16_t get_current_100times(uint16_t adcValue);

#endif /* MONITOR_H_ */