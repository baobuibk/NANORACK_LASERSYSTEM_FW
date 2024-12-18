// spi.c
//
// SPI master routines were pulled from the Atmel ATMega168 datasheet.

#include "spi.h"

// Initialize the SPI as master
void SPI_Init(void)
{
	SPI_PORT |= (1<<SPI_MOSI_PIN)|(1<<SPI_SCK_PIN);
	SPI_DDR |= (1<<SPI_MOSI_PIN)|(1<<SPI_SCK_PIN)|(1<<SW35_PIN);
	SPCR = (1<<SPE)|(1<<MSTR);
	SPSR = 0;
}

// Transfer a byte of data
uint8_t SPI_SendByte( uint8_t data )
{
	// Start transmission
	SPDR = data;
	
	while (!(SPSR & (1 << SPIF)));
	return SPDR;
}


// Transfer a byte of data
uint8_t SPI_ReadByte( void )
{
	// Start transmission
	SPDR = 0xFF;
	while (!(SPSR & (1 << SPIF)));
	return SPDR;
}

