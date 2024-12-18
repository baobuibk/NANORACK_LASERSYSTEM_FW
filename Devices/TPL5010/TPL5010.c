/*
 * TPL5010.c
 *
 * Created: 5/20/2024 1:55:43 PM
 *  Author: Admin
 */ 
#include <avr/io.h>
#include "TPL5010.h"
#include "Laserboard.h"
void	TPL5010_init (void)
{
	TPL5010_DDR |= (1 << TPL5010_DONE_PIN);
};

void TPL5010_trig_on(void)
{
	TPL5010_PORT |= (1 << TPL5010_DONE_PIN);
}

void TPL5010_trig_off(void)
{
	TPL5010_PORT &= ~(1 << TPL5010_DONE_PIN);
}