/*
 * MCP4902.h
 *
 * Created: 11/23/2024 11:18:22 PM
 *  Author: DELL
 */ 


#ifndef MCP4902_H_
#define MCP4902_H_

#include "spi.h"

#define _VREF_DAC		5.0f


#define MCP_BUF_BIT		14
#define MCP_GA_BIT		13
#define MCP_SHDN_BIT	12
#define MCP_AB_BIT		15

#define DAC_CHA			0
#define DAC_CHB			1


void DAC_Init(void);
uint8_t v2dac(uint16_t voltage);
uint16_t dac2v(uint8_t dac);
void DAC_Write(uint8_t channel, uint8_t DAC_Data);
void DAC_Off(uint8_t channel);
#endif /* MCP4902_H_ */