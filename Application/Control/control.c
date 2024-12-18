/*
 * control.c
 *
 * Created: 11/24/2024 2:46:21 AM
 *  Author: DELL
 */ 
#include "control.h"


static void control_task_update(void);
Command_Typedef s_Control;

static port_state_t PortMap[48] = 
{
	PORT_L, PORT_L, PORT_B, PORT_H, PORT_E, PORT_E, PORT_J, PORT_A,	//0-7
	PORT_D, PORT_L, PORT_B, PORT_H, PORT_E, PORT_E, PORT_J, PORT_G,	//8-15
	PORT_G, PORT_L, PORT_B, PORT_H, PORT_E, PORT_E, PORT_J, PORT_J,	//16-23
	PORT_D, PORT_L, PORT_H, PORT_H, PORT_H, PORT_G, PORT_C, PORT_J,	//24-31
	PORT_D, PORT_L, PORT_G, PORT_B, PORT_H, PORT_C, PORT_C, PORT_J,	//32-39
	PORT_D, PORT_D, PORT_G, PORT_B, PORT_H, PORT_C, PORT_C, PORT_J	//40-47
};

static uint8_t	PinMap[48] = 
{
	SW0_PIN, SW1_PIN, SW2_PIN, SW3_PIN, SW4_PIN, SW5_PIN, SW6_PIN, SW7_PIN,
	SW8_PIN, SW9_PIN, SW10_PIN, SW11_PIN, SW12_PIN, SW13_PIN, SW14_PIN, SW15_PIN,
	SW16_PIN, SW17_PIN, SW18_PIN, SW19_PIN, SW20_PIN, SW21_PIN, SW22_PIN, SW23_PIN,
	SW24_PIN, SW25_PIN, SW26_PIN, SW27_PIN, SW28_PIN, SW29_PIN, SW30_PIN, SW31_PIN,
	SW32_PIN, SW33_PIN, SW34_PIN, SW35_PIN, SW36_PIN, SW37_PIN, SW38_PIN, SW39_PIN,
	SW40_PIN, SW41_PIN, SW42_PIN, SW43_PIN, SW44_PIN, SW45_PIN, SW46_PIN, SW47_PIN
};

typedef struct _Control_TaskContextTypedef_
{
	SCH_TASK_HANDLE               taskHandle;
	SCH_TaskPropertyTypedef       taskProperty;
} Control_TaskContextTypedef;


static Control_TaskContextTypedef           s_control_task_context =
{
	SCH_INVALID_TASK_HANDLE,                 // Will be updated by Schedular
	{
		SCH_TASK_SYNC,                      // taskType;
		SCH_TASK_PRIO_0,                    // taskPriority;
		100,                                // taskPeriodInMS;
		control_task_update                // taskFunction;
	}
};


void control_init(void)
{
	// Internal Laser
	DDRA |= (1<<SW7_PIN);
	DDRB |= (1<<SW2_PIN)|(1<<SW10_PIN)|(1<<SW18_PIN)|(1<<SW35_PIN)|(1<<SW43_PIN);
	DDRC |= (1<<SW30_PIN)|(1<<SW37_PIN)|(1<<SW38_PIN)|(1<<SW45_PIN)|(1<<SW46_PIN);
	DDRD |= (1<<SW8_PIN)|(1<<SW24_PIN)|(1<<SW32_PIN)|(1<<SW40_PIN)|(1<<SW41_PIN);
	DDRE |= (1<<SW4_PIN)|(1<<SW5_PIN)|(1<<SW12_PIN)|(1<<SW13_PIN)|(1<<SW20_PIN)|(1<<SW21_PIN);
	DDRG |= (1<<SW15_PIN)|(1<<SW16_PIN)|(1<<SW29_PIN)|(1<<SW34_PIN)|(1<<SW42_PIN);
	DDRH |= (1<<SW3_PIN)|(1<<SW11_PIN)|(1<<SW19_PIN)|(1<<SW26_PIN)|(1<<SW27_PIN)|(1<<SW28_PIN)|(1<<SW36_PIN)|(1<<SW44_PIN);
	DDRJ |= (1<<SW6_PIN)|(1<<SW14_PIN)|(1<<SW22_PIN)|(1<<SW23_PIN)|(1<<SW31_PIN)|(1<<SW39_PIN)|(1<<SW47_PIN);
	DDRL |= (1<<SW0_PIN)|(1<<SW1_PIN)|(1<<SW9_PIN)|(1<<SW17_PIN)|(1<<SW25_PIN)|(1<<SW33_PIN);
	
	// External Laser
	DDRC |= (1<<SWEXT1_PIN)|(1<<SWEXT2_PIN)|(1<<SWEXT3_PIN);
	DDRG |= (1<<SWEXT0_PIN);
	
	DAC_Init();
}

void sw_int_handle(uint8_t sw, sw_state_t sw_state)
{
	if(!sw) return;
	sw--;
	switch(PortMap[sw])
	{
		case PORT_A: PORTA = (sw_state)? (PORTA|(1<<PinMap[sw])):(PORTA&~(1<<PinMap[sw]));break;
		case PORT_B: PORTB = (sw_state)? (PORTB|(1<<PinMap[sw])):(PORTB&~(1<<PinMap[sw]));break;
		case PORT_C: PORTC = (sw_state)? (PORTC|(1<<PinMap[sw])):(PORTC&~(1<<PinMap[sw]));break;
		case PORT_D: PORTD = (sw_state)? (PORTD|(1<<PinMap[sw])):(PORTD&~(1<<PinMap[sw]));break;
		case PORT_E: PORTE = (sw_state)? (PORTE|(1<<PinMap[sw])):(PORTE&~(1<<PinMap[sw]));break;
		case PORT_G: PORTG = (sw_state)? (PORTG|(1<<PinMap[sw])):(PORTG&~(1<<PinMap[sw]));break;
		case PORT_H: PORTH = (sw_state)? (PORTH|(1<<PinMap[sw])):(PORTH&~(1<<PinMap[sw]));break;
		case PORT_J: PORTJ = (sw_state)? (PORTJ|(1<<PinMap[sw])):(PORTJ&~(1<<PinMap[sw]));break;
		case PORT_L: PORTL = (sw_state)? (PORTL|(1<<PinMap[sw])):(PORTL&~(1<<PinMap[sw]));break;	
	}
}

void sw_ext_handle(uint8_t sw, sw_state_t sw_state)
{
	if(!sw) return;
	sw--;
	if(!sw)
	{
		PORTG = (sw_state)? (PORTG|(1<<SWEXT0_PIN)):(PORTG&~(1<<SWEXT0_PIN));
	}
	else
	{
		PORTC = (sw_state)? (PORTC|(1<<(sw-1))):(PORTC&~(1<<(sw-1)));
	}
}


uint8_t abc;
static	void	control_task_update(void)
{

	if(s_Command.sw_int_rqt||i2c_write_rqt)
	{
		if(i2c_write_rqt)
		{
			s_Control.sw_int = rxbuffer[I2C_SW_INT_ADDR];
			s_Control.dac_volt_int = dac2v(rxbuffer[I2C_DAC_INT_ADDR]);
		}
		else if(s_Command.sw_int_rqt)
		{
			s_Control.sw_int = s_Command.sw_int;
			s_Control.dac_volt_int = s_Command.dac_volt_int;
			s_Command.sw_int_rqt = false;
		}
		
		sw_int_handle(s_Control.sw_int_last_on, STATE_OFF);
		
		if(!s_Control.sw_int)
		{
			DAC_Off(DAC_CHA);
			//abc  = 1;
		}
		else
		{
			DAC_Write(DAC_CHA, v2dac(s_Control.dac_volt_int));
			//abc  = 1;
			//sw_int_handle(s_Control.sw_int_last_on, STATE_OFF);
			sw_int_handle(s_Control.sw_int, STATE_ON);
		}
//		s_Command.sw_int_rqt = false;
		if(s_Control.sw_int_last_on != s_Control.sw_int)	s_Control.sw_int_last_on = s_Control.sw_int;
	}
	
	if(s_Command.sw_ext_rqt||i2c_write_rqt)
	{
		if(i2c_write_rqt)
		{
			s_Control.sw_ext = rxbuffer[I2C_SW_EXT_ADDR];
			s_Control.dac_volt_ext = dac2v(rxbuffer[I2C_DAC_EXT_ADDR]);
		}
		else if(s_Command.sw_ext_rqt)
		{
			s_Control.sw_ext = s_Command.sw_ext;
			s_Control.dac_volt_ext = s_Command.dac_volt_ext;
			s_Command.sw_ext_rqt = false;
		}
		
		sw_ext_handle(s_Control.sw_ext_last_on, STATE_OFF);
		
		if(!s_Control.sw_ext)
		{
			DAC_Off(DAC_CHB);
			//abc  = 1;
		}
		else
		{
			DAC_Write(DAC_CHB, v2dac(s_Control.dac_volt_ext));
			//abc  = 1;
			//sw_int_handle(s_Control.sw_int_last_on, STATE_OFF);
			sw_ext_handle(s_Control.sw_ext, STATE_ON);
		}
//		s_Command.sw_ext_rqt = false;
		if(s_Control.sw_ext_last_on != s_Control.sw_ext)	s_Control.sw_ext_last_on = s_Control.sw_ext;
		if(i2c_write_rqt) i2c_write_rqt = false;
	}
	
	
}

void control_create_task(void)
{
	control_init();
	SCH_TASK_CreateTask(&s_control_task_context .taskHandle, &s_control_task_context .taskProperty);
}
