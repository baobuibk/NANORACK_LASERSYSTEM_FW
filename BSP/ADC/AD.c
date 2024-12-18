#include "AD.h"


ADC_CurrentDataTypedef ADC_Data;

void ADC_Init(void)
{
	ADMUX = (1<<REFS0);
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN);
	MONITOR_DDR &= ~((1<<I_MO_EXT)|(1<<I_MO_INT));
	MONITOR_PORT &= ~((1<<I_MO_EXT)|(1<<I_MO_INT));
	ADCSRA |= (1<<ADIE);
	sei();
}


void Set_Channel(uint8_t channel)
{
	ADMUX &= 0xE0;
	ADMUX |= channel;
};

ISR(ADC_vect)
{
	uint8_t channel = ADC_Data.Channel;
	ADC_Data.Data[channel] = ADCW;
	channel++;
	if(channel > 1) channel = 0;
	Set_Channel(channel);
	ADC_Data.Channel = channel;
	ADC_Data.Wait = MUX_WAIT_FOR_STABLE;
}