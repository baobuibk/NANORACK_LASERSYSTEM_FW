/*
 * PMU_timer.c
 *
 * Created: 5/15/2024 5:16:05 PM
 *  Author: Admin
 */ 
#include "timer.h"

void	timer_start_timer0(void)
{	
	TCCR0B = (1<<CS01)|(1<<CS00);				//DIV = 64
	TIMSK0 |= (1<<OCIE0A);
	sei();
}
void timer_stop_timer0(void)
{
	TCCR0B &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));
}
void timer_timer0_init(void)
{
	TCCR0A = (1<<WGM01);
	TCNT0 = 0;
	OCR0A = 125 - 1;
}

/////////////////////////////////////////////// Timer 1 //////////////////////////////////////////////////

// void	timer_start_timer1(void)
// {
// 	TCCR1B |= (1<<CS11);				//DIV = 8 => Tclk = 1us
// }
// void timer_stop_timer1(void)
// {
// 	TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
// }
// void timer_timer1_init(void)
// {
// 	TCCR1A |= (1<<COM1A1)|(1<<COM1B1)|(1<<WGM11);	
// 	TCCR1B |= (1<<WGM13)|(1<<WGM12);						//Fast PWM (mode 14)
// 	ICR1 = 100;												
// 	OCR1A = 0;
// 	OCR1B = 0;
// }
