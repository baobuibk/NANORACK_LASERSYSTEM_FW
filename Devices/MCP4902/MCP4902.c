/*
 * MCP4902.c
 *
 * Created: 11/23/2024 11:18:34 PM
 *  Author: DELL
 */ 
#include "MCP4902.h"

void DAC_Init(void)
{
	DAC_CS_PORT |= (1<<DAC_CS_PIN);
	DAC_LATCH_PORT |= (1<<DAC_LATCH_PIN);
	
	DAC_CS_DDR |= (1<<DAC_CS_PIN);
	DAC_LATCH_DDR |= (1<<DAC_LATCH_PIN);
}

uint8_t v2dac(uint16_t voltage)
{
	return (voltage * 2.55f)/_VREF_DAC;
}

uint16_t dac2v(uint8_t dac)
{
	return (dac*_VREF_DAC)/2.55f;
}

void DAC_Write(uint8_t channel, uint8_t DAC_Data)
{
	DAC_CS_PORT &= ~(1<<DAC_CS_PIN);
	uint16_t temp = channel ? ((1<<MCP_AB_BIT)|(1<<MCP_GA_BIT)|(1<<MCP_SHDN_BIT)|(DAC_Data<<4)):((1<<MCP_GA_BIT)|(1<<MCP_SHDN_BIT)|(DAC_Data<<4));
	
	SPI_SendByte((uint8_t)(temp >> 8));
	//asm volatile("nop");
	SPI_SendByte((uint8_t)temp);
	DAC_CS_PORT |= (1<<DAC_CS_PIN);
	DAC_LATCH_PORT &= ~(1<<DAC_LATCH_PIN);
	//asm volatile("nop");
	DAC_LATCH_PORT |= (1<<DAC_LATCH_PIN);
}

void DAC_Off(uint8_t channel)
{
	DAC_CS_PORT &= ~(1<<DAC_CS_PIN);
	uint16_t temp = channel ? ((1<<MCP_AB_BIT)|(1<<MCP_GA_BIT)):(1<<MCP_GA_BIT);
	SPI_SendByte((uint8_t)(temp >> 8));
	//asm volatile("nop");
	SPI_SendByte((uint8_t)temp);
	DAC_CS_PORT |= (1<<DAC_CS_PIN);
	DAC_LATCH_PORT &= ~(1<<DAC_LATCH_PIN);
	//asm volatile("nop");
	DAC_LATCH_PORT |= (1<<DAC_LATCH_PIN);
}