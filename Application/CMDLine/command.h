/*
 * command.h
 *
 * Created: 5/19/2024 8:38:57 PM
 *  Author: Admin
 */ 


#ifndef COMMAND_H_
#define COMMAND_H_

#include "cmdline.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "scheduler.h"
#include "measurement.h"
#include "eeprom.h"
#include <AVR/pgmspace.h>

typedef struct _Command_Typedef_
{
	uint8_t sw_int;
	bool sw_int_rqt;
	uint8_t sw_int_last_on;
	uint16_t dac_volt_int;
	
	uint8_t sw_ext;
	bool sw_ext_rqt;
	uint8_t sw_ext_last_on;
	uint16_t dac_volt_ext;
}Command_Typedef;

extern Command_Typedef s_Command;
#define	COMMAND_MAX_LENGTH	32
#define MAX_PRECISION	(2)

void	command_init(void);
void	command_send_splash(void);

void	command_create_task(void);
char * ftoa(double f, char * buf, int precision);

int	Cmd_help(int argc, char *argv[]);
int	Cmd_get_current(int argc, char *argv[]);
int	Cmd_set_dac(int argc, char *argv[]);
int	Cmd_set_laser(int argc, char *argv[]);

#endif /* COMMAND_H_ */