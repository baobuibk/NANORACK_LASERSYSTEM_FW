/*
 * slave.h
 *
 * Created: 11/24/2024 11:30:14 PM
 *  Author: DELL
 */ 


#ifndef SLAVE_H_
#define SLAVE_H_

#include "i2c.h"
#include "scheduler.h"
#include "command.h"
#include "control.h"
void slave_init(void);
void slave_create_task(void);

#endif /* SLAVE_H_ */