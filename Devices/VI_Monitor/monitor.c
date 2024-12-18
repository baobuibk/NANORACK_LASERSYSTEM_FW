/*
 * monitor.c
 *
 * Created: 5/27/2024 10:30:40 PM
 *  Author: DELL
 */ 

#include "monitor.h"




float voltage_cal(uint16_t adcValue, uint16_t res_up, uint16_t res_low)
{
	float vadc = (adcValue * (double)_VREF_ADC) / (double)_ADC_MAX;
	float voltage = vadc * 1.0f * (res_up + res_low);
	voltage /= 1.0f * res_low;
	return voltage;
}

float current_cal(uint16_t adcValue)
{
	float vadc = (adcValue * (double)_VREF_ADC) / (double)_ADC_MAX;
	return (vadc * 1000.0f) / ((double)_R_SHUNT);
}


uint16_t	get_voltage_100times(uint16_t adcValue, uint16_t res_up, uint16_t res_low)
{
	return	100*voltage_cal(adcValue, res_up, res_low);
}

uint16_t	get_current_100times(uint16_t adcValue)
{
	return	current_cal(adcValue);
}