/*
 * TPL5010.h
 *
 * Created: 5/20/2024 1:55:26 PM
 *  Author: Admin
 */ 


#ifndef TPL5010_H_
#define TPL5010_H_

#define TIME_TRIGGER 2000			//2000ms
#define TIME_TRIGGER_ON 200			

uint16_t TPL5010_timer;


void	TPL5010_init (void);
void TPL5010_trig_on(void);
void TPL5010_trig_off(void);


#endif /* TPL5010_H_ */