
#include "i2c.h"


static volatile uint8_t status_count = 0;
volatile bool i2c_write_rqt = false;

void i2c_init ( void )
{
    TWCR = 1 << TWEN ;
    TWBR = ( F_CPU / I2C_CONFIG_F_SCL - 16 ) / 2;
}

static void i2c_wait ( void )
{
    while ( !( TWCR &  (1 << TWINT ) ) );
}

/**
 * Send I2C Start Condition
 */
int8_t i2c_start ( void )
{
    TWCR =  (1 << TWEN ) | (1 << TWINT ) |  (1 << TWSTA );
    i2c_wait (  );
    return ( TWSR & TW_STATUS_MASK ) != TW_START;
}

/**
 * Send I2C Repeated-Start Condition
 */
int8_t i2c_restart ( void )
{
    TWCR =  (1 << TWEN ) |  (1 << TWINT ) |  (1 << TWSTA );
    i2c_wait (  );
    return ( TWSR & TW_STATUS_MASK ) != TW_REP_START;
}

/**
 * Send I2C Stop Condition
 */
int8_t i2c_stop ( void )
{
    TWCR =  ( 1 << TWEN ) |  (1 << TWINT ) |  (1 << TWSTO );
    while (TWCR &  (1 << TWSTO));
    return 0;
}


int8_t i2c_addr ( uint8_t addr )
{
    TWDR = addr;
    TWCR =  (1 << TWEN ) |  (1 << TWINT );
    i2c_wait (  );
    return ( TWSR & TW_STATUS_MASK ) != ( ( addr & 1 ) ? TW_MR_SLA_ACK : TW_MT_SLA_ACK );
}


int8_t i2c_tx_byte ( uint8_t byte )
{
    TWDR = byte;
    TWCR =  ( 1 << TWEN ) |  (1 << TWINT );
    i2c_wait (  );
    return ( TWSR & TW_STATUS_MASK ) != TW_MT_DATA_ACK;
}


int8_t i2c_tx_data ( const uint8_t * data, size_t len )
{
    size_t i;

    for ( i = 0; i < len; i++ )
    {
        if ( i2c_tx_byte ( data[i] ) )
        {
            return 1;
        }
    }

    return 0;
}


int8_t i2c_rx_byte ( uint8_t * byte )
{
    TWCR =  (1 << TWEN ) |  (1 << TWINT ) |  (1 << TWEA );
    i2c_wait (  );
    *byte = TWDR;
    return ( TWSR & TW_STATUS_MASK ) != TW_MR_DATA_ACK;
}


int8_t i2c_rx_last ( uint8_t * byte )
{
    TWCR =  (1 << TWEN ) |  (1 << TWINT );
    i2c_wait (  );
    *byte = TWDR;
    return ( TWSR & TW_STATUS_MASK ) != TW_MR_DATA_NACK;
}

/**
 * Receive data bytes from Slave Device
 */
int8_t i2c_rx_data ( uint8_t * data, size_t len )
{
    size_t i;

    for ( i = 0; i + 1 < len; i++ )
    {
        if ( i2c_rx_byte ( data + i ) )
        {
            return 1;
        }
    }

    if ( len )
    {
        if ( i2c_rx_last ( data + i ) )
        {
            return 1;
        }
    }

    return 0;
}


/************************************************  Slave  ********************************************************/

void i2c_slave_init(uint8_t address)
{
	//cli();
	TWAR = address<<1;
	TWCR = (1<<TWIE) | (1<<TWEA) | (1<<TWINT) | (1<<TWEN);
	sei();
}

uint8_t tesst;
ISR(TWI_vect){
	
	uint8_t data;
	tesst = 0;
	switch(TW_STATUS)
	{
		/* ################################################################################# */
		/* ################################# SLAVE RECEIVE ################################# */
		/* ################################################################################# */
		
		/************************ Master start write ************************/
		case TW_SR_SLA_ACK:
			buffer_address = 0xFF;
			status_count = 0;
			TWCR |=  (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
		break;
		
		/************************ Slave received data and then sent ack ************************/
		case TW_SR_DATA_ACK:
			data = TWDR;
			status_count++;
			if(buffer_address == 0xFF)			// First byte write is address
			{
				buffer_address = data;
			}
			else
			{
				rxbuffer[buffer_address++] = data;
			}
			
			if(buffer_address > I2C_RX_BUFFER_MAX - 1)
				TWCR |=  (1<<TWIE) | (1<<TWINT) | (1<<TWEN);
			else
				TWCR |=  (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
		break;
		
		/************************ Slave stop ************************/
		case TW_SR_STOP:
			if(status_count > 1)	i2c_write_rqt = true;
			TWCR |=  (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);			
		break;
		
		/* ################################################################################## */
		/* ################################# SLAVE TRANSMIT ################################# */
		/* ################################################################################## */
		
		/************************ Master start read ************************/
		case TW_ST_SLA_ACK:
			TWDR = txbuffer[buffer_address++];
			TWCR |=  (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
		break;
		
		/************************ Master received and sent ack ************************/
		case TW_ST_DATA_ACK:
			TWDR = txbuffer[buffer_address++];
			TWCR |=  (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
		break;
		
		/************************ Slave stop ************************/
		case TW_ST_DATA_NACK:
			TWCR |=  (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
		break;
		
		default:
			TWCR |=  (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
		break;
		
	}

	//TW_STATUS_LAST = TW_STATUS;


}

