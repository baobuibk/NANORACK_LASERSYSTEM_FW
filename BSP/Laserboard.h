/*
 * Laserboard.h
 *
 * Created: 11/22/2024 2:54:52 PM
 *  Author: DELL
 */ 


#ifndef LASERBOARD_H_
#define LASERBOARD_H_

#define F_CPU 8000000
#include <stddef.h>
#include <basetypedef.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/atomic.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <util/twi.h>

#define MONITOR_DDR		DDRF
#define MONITOR_PORT	PORTF

#define I_MO_INT		PINF0
#define I_MO_EXT		PINF1
/**********************************/

#define LED1_DDR		DDRK
#define LED1_PORT		PORTK

#define LED1_PIN		PINK7

/**********************************/

#define SPI_DDR			DDRB
#define SPI_PORT		PORTB

#define SPI_SCK_PIN		PINB1
#define SPI_MOSI_PIN	PINB2
#define SPI_MISO_PIN	PINB3

/**********************************/

#define USART0_DDR		DDRE
#define USART0_PORT		PORTE
#define USART0_RXD_PIN	PINE0
#define USART0_TXD_PIN	PINE1

/**********************************/

#define TWI_DDR			DDRD
#define TWI_PORT		PORTD

#define TWI_SCL_PIN		PIND0
#define TWI_SDA_PIN		PIND1

/**********************************/

#define CAN_INT_DDR		DDRD
#define CAN_INT_PORT	PORTD
#define CAN_INT_PIN		PIND2

/**********************************/

#define CAN_CS_DDR		DDRJ
#define CAN_CS_PORT		PORTJ
#define CAN_CS_PIN		PINJ7

/**********************************/

#define DAC_CS_DDR		DDRA
#define DAC_CS_PORT		PORTA
#define DAC_CS_PIN		PINA0

/**********************************/

#define DAC_LATCH_DDR		DDRK
#define DAC_LATCH_PORT		PORTK
#define DAC_LATCH_PIN		PINK4

/**********************************/

#define TPL5010_DDR			DDRL
#define TPL5010_PORT		PORTL
#define TPL5010_WAKE_PIN	PINL2
#define TPL5010_DONE_PIN	PINL3

/**********************************/

#define SW0_PIN				PINL1
#define SW1_PIN				PINL0
#define SW2_PIN				PINB5
#define SW3_PIN				PINH3
#define SW4_PIN				PINE5
#define SW5_PIN				PINE4
#define SW6_PIN				PINJ2
#define SW7_PIN				PINA7

#define SW8_PIN				PIND7
#define SW9_PIN				PINL4
#define SW10_PIN			PINB6
#define SW11_PIN			PINH4
#define SW12_PIN			PINE6
#define SW13_PIN			PINE3
#define SW14_PIN			PINJ1
#define SW15_PIN			PING2

#define SW16_PIN			PING0
#define SW17_PIN			PINL5
#define SW18_PIN			PINB7
#define SW19_PIN			PINH5
#define SW20_PIN			PINE7
#define SW21_PIN			PINE2
#define SW22_PIN			PINJ0
#define SW23_PIN			PINJ6

#define SW24_PIN			PIND6
#define SW25_PIN			PINL6
#define SW26_PIN			PINH7
#define SW27_PIN			PINH6
#define SW28_PIN			PINH0
#define SW29_PIN			PING5
#define SW30_PIN			PINC7
#define SW31_PIN			PINJ5

#define SW32_PIN			PIND5
#define SW33_PIN			PINL7
#define SW34_PIN			PING3
#define SW35_PIN			PINB0
#define SW36_PIN			PINH1
#define SW37_PIN			PINC4
#define SW38_PIN			PINC6
#define SW39_PIN			PINJ4

#define SW40_PIN			PIND4
#define SW41_PIN			PIND3
#define SW42_PIN			PING4
#define SW43_PIN			PINB4
#define SW44_PIN			PINH2
#define SW45_PIN			PINC3
#define SW46_PIN			PINC5
#define SW47_PIN			PINJ3

#define SWEXT0_PIN			PING1
#define SWEXT1_PIN			PINC0
#define SWEXT2_PIN			PINC1
#define SWEXT3_PIN			PINC2

#endif /* LASERBOARD_H_ */