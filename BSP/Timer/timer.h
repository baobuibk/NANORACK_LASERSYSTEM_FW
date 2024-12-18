/*
 * PMU_timer.h
 *
 * Created: 5/15/2024 5:15:48 PM
 *  Author: Admin
 */ 


#ifndef TIMER_H_
#define TIMER_H_


#include "Laserboard.h"

void	timer_start_timer0(void);
void	timer_stop_timer0(void);
void	timer_timer0_init(void);

// void	timer_start_timer1(void);
// void timer_stop_timer1(void);
// void timer_timer1_init(void);
// void setPWM(uint8_t channel, uint8_t PWM);

#endif /* TIMER_H_ */