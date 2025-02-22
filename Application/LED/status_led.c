/*
 * status_led.c
 *
 * Created: 5/15/2024 2:22:51 PM
 *  Author: Admin
 */ 
#include "status_led.h"

/* Private define ------------------------------------------------------------*/
#define	POWERUP_PERIOD	500
#define	POWER_NORMAL_OFF_PERIOD	3000
#define	POWER_NORMAL_ON_PERIOD	500
void	status_led_update(void);
static	void	status_led_normal(void);
/* Private typedef -----------------------------------------------------------*/
typedef struct Led_TaskContextTypedef
{
	SCH_TASK_HANDLE               taskHandle;
	SCH_TaskPropertyTypedef       taskProperty;
} Led_TaskContextTypedef;

typedef	struct StatusLed_CurrentStateTypedef
{
	uint8_t				led1:1;
	//uint8_t				led2:1;
	Led_DisplayStateTypedef	state:6;	
}StatusLed_CurrentStateTypedef;
	
/* Private variables ---------------------------------------------------------*/

static	StatusLed_CurrentStateTypedef	s_led_display_status = {0,POWERUP};
static Led_TaskContextTypedef           s_task_context =
{
	SCH_INVALID_TASK_HANDLE,                 // Will be updated by Schedular
	{
		SCH_TASK_SYNC,                      // taskType;
		SCH_TASK_PRIO_0,                    // taskPriority;
		500,                                // taskPeriodInMS;
		status_led_update                // taskFunction;
	}
};

/* Private function prototypes -----------------------------------------------*/

static void status_led_led1_on(void);
static void status_led_led1_off(void);
static	void	status_led_powerup(void);
void	status_led_set_status(Led_DisplayStateTypedef status)
{
	s_led_display_status.state = status;
}

void status_led_init(void)
{
	LED1_DDR |= (1 << LED1_PIN);
	LED1_PORT &= ~((1 << LED1_PIN));
	s_led_display_status.led1 = 0;
	s_led_display_status.state = POWERUP;
	status_led_led1_off();	
}

static void status_led_led1_off(void)
{
	LED1_PORT |= ((1 << LED1_PIN));	
}

static void status_led_led1_on(void)
{
	LED1_PORT &= ~((1 << LED1_PIN));
}


void	status_led_update(void)
{
	switch (s_led_display_status.state) {
	case POWERUP:
		status_led_powerup();
		break;
	case NORMAL:
		status_led_normal();
		break;
	case POWERING_SUB:
		break;
	case OVERCURRENT:
		break;	
	}
}
static	void	status_led_powerup(void)
{
		{
			if (s_led_display_status.led1 == 1)// && (s_led_display_status.led1 == 1))	//both ON
			{
				if (SCH_TIM_HasCompleted(SCH_TIM_LED))
				{
					s_led_display_status.led1 = 0;
					status_led_led1_off();
					SCH_TIM_Start(SCH_TIM_LED,POWERUP_PERIOD);	//restart
				}
			}
			else if (s_led_display_status.led1 == 0)// && (s_led_display_status.led1 == 0))	//both OFF
			{
				if (SCH_TIM_HasCompleted(SCH_TIM_LED))
				{
					s_led_display_status.led1 = 1;
					status_led_led1_on();
					SCH_TIM_Start(SCH_TIM_LED,POWERUP_PERIOD);	//restart					
					
				}
			}
			else
			{
				s_led_display_status.led1 = 0;
				status_led_led1_off();
				SCH_TIM_Start(SCH_TIM_LED,POWERUP_PERIOD);	//restart
			}
		}
}

static	void	status_led_normal(void)
{
	if (s_led_display_status.led1 == 1)
	{
		if (SCH_TIM_HasCompleted(SCH_TIM_LED))
			{
				s_led_display_status.led1 = 0;
				status_led_led1_off();
				SCH_TIM_Start(SCH_TIM_LED,POWER_NORMAL_OFF_PERIOD);	//restart
			}		
	}

}

void	status_led_create_task(void)
{
	status_led_init();
	SCH_TASK_CreateTask(&s_task_context.taskHandle, &s_task_context.taskProperty);
}