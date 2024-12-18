/*
 * eeprom.h
 *
 * Created: 6/22/2024 12:48:40 AM
 *  Author: DELL
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_
#include <avr/eeprom.h>

#define HIGH_TEMP_POINT_ADR	(uint16_t *)0x40

#define LOW_TEMP_POINT_ADR	(uint16_t *)0x42

#define HEATER_PWM_ADR	(uint8_t *)0x44

void eeprom_init(void);

#endif /* EEPROM_H_ */