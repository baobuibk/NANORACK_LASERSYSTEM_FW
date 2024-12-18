
#ifndef AD_H_
#define AD_H_

#include "Laserboard.h"

#define MUX_WAIT_FOR_STABLE 5
#define ADC_CURRENT_INT	0
#define ADC_CURRENT_EXT	1

typedef	struct _ADC_CurrentDataTypedef_
{
	uint16_t				Data[2];
	uint8_t					Channel;
	uint8_t					Wait;
}ADC_CurrentDataTypedef;

extern ADC_CurrentDataTypedef ADC_Data;

void ADC_Init(void);
void Set_Channel(uint8_t channel);

#endif /* AD_H_ */
