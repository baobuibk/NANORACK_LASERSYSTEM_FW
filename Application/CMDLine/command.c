/*
 * command.c
 *
 * Created: 5/19/2024 8:37:53 PM
 *  Author: Admin
 */ 
#include "command.h"



const char cmd_line_1[] PROGMEM = ".......................................................\r\n";
const char cmd_line_2[] PROGMEM = "..     ____                       _     _ _          ..\r\n";
const char cmd_line_3[] PROGMEM = "..    / ___| _ __   __ _  ___ ___| |   (_|_)_ __     ..\r\n";
const char cmd_line_4[] PROGMEM = "..    \\___ \\| '_ \\ / _` |/ __/ _ \\ |   | | | '_ \\    ..\r\n";
const char cmd_line_5[] PROGMEM = "..     ___) | |_) | (_| | (_|  __/ |___| | | | | |   ..\r\n";
const char cmd_line_6[] PROGMEM = "..    |____/| .__/ \\__,_|\\___\\___|_____|_|_|_| |_|   ..\r\n";
const char cmd_line_7[] PROGMEM = "..          |_|    ____  __  __  _   _               ..\r\n";
const char cmd_line_8[] PROGMEM = "..                |  _ \\|  \\/  || | | |              ..\r\n";
const char cmd_line_9[] PROGMEM = "..                | |_) | |\\/| || | | |              ..\r\n";
const char cmd_line_10[] PROGMEM = "..                |  __/| |  | || |_| |              ..\r\n";
const char cmd_line_11[] PROGMEM = "..                |_|   |_|  |_| \\___/               ..\r\n";
const char cmd_line_12[] PROGMEM = "..              __     ___   ___   ___               ..\r\n";
const char cmd_line_13[] PROGMEM = "..              \\ \\   / / | / _ \\ / _ \\              ..\r\n";
const char cmd_line_14[] PROGMEM = "..               \\ \\ / /| || | | | | | |             ..\r\n";
const char cmd_line_15[] PROGMEM = "..                \\ V / | || |_| | |_| |             ..\r\n";
const char cmd_line_16[] PROGMEM = "..                 \\_/  |_(_)___(_)___/              ..\r\n";
const char cmd_line_17[] PROGMEM = "..                                                   ..\r\n";
const char cmd_line_18[] PROGMEM = ".......................................................\r\n";


Command_Typedef s_Command;

/* Private typedef -----------------------------------------------------------*/
typedef struct _Command_TaskContextTypedef_
{
	SCH_TASK_HANDLE               taskHandle;
	SCH_TaskPropertyTypedef       taskProperty;
} Command_TaskContextTypedef;


//typedef enum{OK = 0, CMDLINE_BAD_CMD, CMDLINE_TOO_MANY_ARGS, CMDLINE_TOO_FEW_ARGS, CMDLINE_INVALID_ARG} command_error_code_t;
const char * ErrorCode[5] = {"OK\r\n", "CMDLINE_BAD_CMD\r\n", "CMDLINE_TOO_MANY_ARGS\r\n",
"CMDLINE_TOO_FEW_ARGS\r\n", "CMDLINE_INVALID_ARG\r\n" };

static	void	command_task_update(void);
tCmdLineEntry g_psCmdTable[] = {{ "help", Cmd_help, " : format: help"},					
								{"get_current", Cmd_get_current , " : format: get_current [int/ext]"},	
								{"set_laser", Cmd_set_laser, " : format: set_laser [int/ext] [channel] [dac voltage]"},					
								{0,0,0}
								};

volatile static	ringbuffer_t *p_CommandRingBuffer;
static	char s_commandBuffer[COMMAND_MAX_LENGTH];
static uint8_t	s_commandBufferIndex = 0;


static Command_TaskContextTypedef           s_CommandTaskContext =
{
	SCH_INVALID_TASK_HANDLE,                 // Will be updated by Schedular
	{
		SCH_TASK_SYNC,                      // taskType;
		SCH_TASK_PRIO_0,                    // taskPriority;
		10,                                // taskPeriodInMS;
		command_task_update                // taskFunction;
	}
};

void	command_init(void)
{
	usart0_init();
	p_CommandRingBuffer = uart_get_uart0_rx_buffer_address();
	memset((void *)s_commandBuffer, 0, sizeof(s_commandBuffer));
	s_commandBufferIndex = 0;
	usart0_send_string("LASER DIODE FIRMWARE V1.0.0 \r\n");
	command_send_splash();
}

static const double rounders[MAX_PRECISION + 1] =
{
	0.5,				// 0
	0.05,				// 1
	0.005,				// 2
};

char * ftoa(double f, char * buf, int precision)
{
	char * ptr = buf;
	char * p = ptr;
	char * p1;
	char c;
	long intPart;
	if (precision > MAX_PRECISION)
	precision = MAX_PRECISION;
	if (f < 0)
	{
		f = -f;
		*ptr++ = '-';
	}
	if (precision < 0)  
	{
		if (f < 1.0) precision = 6;
		else if (f < 10.0) precision = 5;
		else if (f < 100.0) precision = 4;
		else if (f < 1000.0) precision = 3;
		else if (f < 10000.0) precision = 2;
		else if (f < 100000.0) precision = 1;
		else precision = 0;
	}
	if (precision)
	f += rounders[precision];
	intPart = f;
	f -= intPart;

	if (!intPart)
	*ptr++ = '0';
	else
	{
		p = ptr;
		while (intPart)
		{
			*p++ = '0' + intPart % 10;
			intPart /= 10;
		}
		p1 = p;
		while (p > ptr)
		{
			c = *--p;
			*p = *ptr;
			*ptr++ = c;
		}
		ptr = p1;
	}
	if (precision)
	{
		*ptr++ = '.';
		while (precision--)
		{
			f *= 10.0;
			c = f;
			*ptr++ = '0' + c;
			f -= c;
		}
	}
	*ptr = 0;
	return buf;
}

static	void	command_task_update(void)
{
	char rxData;
	int8_t	ret_val;
	while (! rbuffer_empty(p_CommandRingBuffer))
	{
		rxData = rbuffer_remove(p_CommandRingBuffer);
		usart0_send_char(rxData);
		if ((rxData == '\r') || (rxData == '\n'))		//got a return or new line
		{
			if (s_commandBufferIndex > 0)		//if we got the CR or LF at the begining, discard	
			{
				s_commandBuffer[s_commandBufferIndex] = 0;
				s_commandBufferIndex++;
				ret_val = CmdLineProcess(s_commandBuffer);		
				s_commandBufferIndex = 0;	
				//usart0_send_string("\r\n");	
				usart0_send_string(ErrorCode[ret_val]);
				usart0_send_string("> ");
			}	
			else usart0_send_string("> ");
		}
		else if ((rxData == 8) || (rxData == 127))	
		{
			if (s_commandBufferIndex > 0) s_commandBufferIndex--;
		}
		else
		{
			s_commandBuffer[s_commandBufferIndex] = rxData;
			s_commandBufferIndex ++;
			if (s_commandBufferIndex >= COMMAND_MAX_LENGTH) s_commandBufferIndex= 0;
		}
	}	
}


int Cmd_help(int argc, char *argv[]) {
	tCmdLineEntry *pEntry;

	usart0_send_string("\nAvailable commands: \r\n");
	pEntry = &g_psCmdTable[0];
	while (pEntry->pcCmd) {
		usart0_send_string(pEntry->pcCmd);
		usart0_send_string(pEntry->pcHelp);
		usart0_send_string("\r\n");
		pEntry++;
	}
	return (CMDLINE_OK);
}



//*****************************************************************************
//
// Format: Cmd_get_current
//
//*****************************************************************************
int	Cmd_get_current(int argc, char *argv[])
{
	if (argc < 2) return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 2) return CMDLINE_TOO_MANY_ARGS;
	
	if(strcmp(argv[1], "int") == 0)
	{
		UARTprintf("Current_internal: %d ", s_Measurement.current_internal);
		usart0_send_string("mA");
	}
	
	else if(strcmp(argv[1], "ext") == 0)
	{
		UARTprintf("Current_external: %d ", s_Measurement.current_external);
		usart0_send_string("mA");
	}
	else return CMDLINE_INVALID_ARG;
	
	usart0_send_string("\r\n");
 	return CMDLINE_OK;
}


//*****************************************************************************
//
// Format: Cmd_set_laser
//
//*****************************************************************************
int Cmd_set_laser(int argc, char *argv[])
{
	uint8_t switching = atoi(argv[2]);
	float temp = atof(argv[3]);
	if(strcmp(argv[1], "int") == 0)
	{
		if(!switching)
		{
			if(argc < 3)		return CMDLINE_TOO_FEW_ARGS;
			else if(argc > 3)	return CMDLINE_TOO_MANY_ARGS;
		}
		
		else if((switching > 0)&&(switching <= 48))
		{
			if(argc < 4)		return CMDLINE_TOO_FEW_ARGS;
			else if(argc > 4)	return CMDLINE_TOO_MANY_ARGS;
			if((temp < 0)||(temp > 4.9))	return CMDLINE_INVALID_ARG;
			s_Command.dac_volt_int = (uint16_t)100*temp;
		}
		else if(switching > 48)	return CMDLINE_INVALID_ARG;
		s_Command.sw_int_rqt = true;
		s_Command.sw_int = switching;
		
	}
	
	else if(strcmp(argv[1], "ext") == 0)
	{
		if(!switching)
		{
			if(argc < 3)		return CMDLINE_TOO_FEW_ARGS;
			else if(argc > 3)	return CMDLINE_TOO_MANY_ARGS;
		}
		
		else if((switching > 0)&&(switching <= 4))
		{
			if(argc < 4)		return CMDLINE_TOO_FEW_ARGS;
			else if(argc > 4)	return CMDLINE_TOO_MANY_ARGS;
			if((temp < 0)||(temp > 4.9))	return CMDLINE_INVALID_ARG;
			s_Command.dac_volt_ext = (uint16_t)100*temp;
		}
		else if(switching > 4)	return CMDLINE_INVALID_ARG;
		
		s_Command.sw_ext_rqt = true;
		s_Command.sw_ext = switching;
		
	}
	
	else return CMDLINE_INVALID_ARG;
 	return CMDLINE_OK;
}


void	command_create_task(void)
{
	command_init();
	SCH_TASK_CreateTask(&s_CommandTaskContext.taskHandle, &s_CommandTaskContext.taskProperty);
}



void	command_send_splash(void)
{
	PGM_P cmd_line_table[] PROGMEM =
	{
		cmd_line_1, cmd_line_2, cmd_line_3, cmd_line_4, cmd_line_5, cmd_line_6,
		cmd_line_7, cmd_line_8, cmd_line_9, cmd_line_10, cmd_line_11, cmd_line_12,
		cmd_line_13, cmd_line_14, cmd_line_15, cmd_line_16, cmd_line_17, cmd_line_18
	};
	
	usart0_send_string("\r\n");
	
	for (unsigned char i = 0; i < 18; i++)	usart0_send_string_P(cmd_line_table[i]);
	usart0_send_string("\r\n> ");
}