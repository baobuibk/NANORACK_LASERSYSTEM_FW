/*
 * eeprom.c
 *
 * Created: 6/22/2024 12:48:28 AM
 *  Author: DELL
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <stdlib.h>
#include "eeprom.h"


void eeprom_init(void)
{
// 	heater_pwm = eeprom_read_byte(HEATER_PWM_ADR);
// 	high_temp_setpoint = eeprom_read_word(HIGH_TEMP_POINT_ADR);
// 	low_temp_setpoint = eeprom_read_word(LOW_TEMP_POINT_ADR);
// 	
// 	if(heater_pwm > 100)				heater_pwm = HEATER_PWM;
// 	if((high_temp_setpoint > 3000)&&(high_temp_setpoint < -1000))	high_temp_setpoint = HIGH_TEMP_THRESH;
// 	if((low_temp_setpoint > 3000)&&(low_temp_setpoint < -1000))	low_temp_setpoint = LOW_TEMP_THRESH;
}