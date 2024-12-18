/* ------------------------------------------------------------------
 * I2C Library for AVR Devices
 * ------------------------------------------------------------------ */

#ifndef I2C_H
#define I2C_H


#include "Laserboard.h"

#define I2C_CONFIG_F_SCL	100000
#define I2C_SLAVE_ADDRESS	0x26
#define I2C_TX_BUFFER_MAX	0x20
#define I2C_RX_BUFFER_MAX	0x20

#define I2C_SW_INT_ADDR			0x00
#define I2C_DAC_INT_ADDR		0x01
#define I2C_SW_EXT_ADDR			0x02
#define I2C_DAC_EXT_ADDR		0x03

#define I2C_CURRENT_INT_ADDR	0x04
#define I2C_CURRENT_EXT_ADDR	0x06

volatile bool i2c_write_rqt;

volatile uint8_t buffer_address;
volatile uint8_t txbuffer[I2C_TX_BUFFER_MAX];
volatile uint8_t rxbuffer[I2C_RX_BUFFER_MAX];

extern void i2c_init ( void );
extern void i2c_init_with_pullups ( void );
extern void i2c_uninit ( void );
extern int8_t i2c_start ( void );
extern int8_t i2c_restart ( void );
extern int8_t i2c_stop ( void );
extern int8_t i2c_addr ( uint8_t addr );
extern int8_t i2c_tx_byte ( uint8_t byte );
extern int8_t i2c_tx_data ( const uint8_t * data, size_t len );
extern int8_t i2c_rx_byte ( uint8_t * byte );
extern int8_t i2c_rx_last ( uint8_t * byte );
extern int8_t i2c_rx_data ( uint8_t * data, size_t len );
extern void i2c_slave_init(uint8_t address);

ISR(TWI_vect);

#endif
