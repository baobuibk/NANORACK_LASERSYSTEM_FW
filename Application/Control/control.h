/*
 * control.h
 *
 * Created: 11/24/2024 2:46:13 AM
 *  Author: DELL
 */ 


#ifndef CONTROL_H_
#define CONTROL_H_

#include "scheduler.h"
#include "MCP4902.h"
#include "command.h"

typedef enum{STATE_OFF, STATE_ON} sw_state_t;
typedef enum
{
	PORT_A, PORT_B, PORT_C, PORT_D, PORT_E, PORT_G, PORT_H, PORT_J, PORT_L
} port_state_t;

typedef enum{NO_RQT, INT_RQT, EXT_RQT} request_state_t;
extern Command_Typedef s_Control;
void control_init(void);
void sw_int_handle(uint8_t sw, sw_state_t sw_state);
void sw_ext_handle(uint8_t sw, sw_state_t sw_state);
void control_create_task(void);

#endif /* CONTROL_H_ */