// MASTER

/*
 * TWI_Master_Uni.c
 *
 * Created: 13-08-2021 21:43:36
 * Author : Kanishk Kaushal
 */ 


//MASTER
#include <avr/io.h>
#define F_CPU 1600000UL
#include <util/delay.h>

/*
void Blink()
{
	
		PORTB |= (1<<PINB0);
		_delay_ms(500);
		PORTB &= ~(1<<PINB0);
		_delay_ms(500);
		
}
*/
void I2C_MSTR_INIT()
{
	TWBR = 0b00000001;
	TWSR &=~((1<<TWPS1)|(1<<TWPS0));
	TWCR |=(1<<TWEN);
	
}

void I2C_START()
{												 // TWINT: TWI Interrupt flag: Set to 1 when we complete our current event like Start,Stop,Transmit,Receive etc.
	TWCR |=(1<<TWINT) |(1<<TWEN) |(1<<TWSTA);	 // TWEN: TWI enable bit
	while(!(TWCR & (1<<TWINT)));				 // TWSTA: TWI Start condition bit
	while((TWSR & (0xF8)) != 0x08);              // TWSR: TWI Status Register, 0x08 is to send start condition and wait for Ack
			                                     // TWCR: TWI Control Register
}	

void I2C_ADDR(unsigned char Addr)
{
	
	TWDR = Addr;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	while((TWSR & 0xF8) != 0x18);
	    
}

void I2C_WRITE(char Data)
{
	TWDR = Data;								 // TWDR: TWI Data Register
	TWCR =(1<<TWINT)|(1<<TWEN);                  // Start Transmission Of Data
	while(!(TWCR & (1<<TWINT)));                 // Transfer Data
	while((TWSR & 0xF8) != 0x28);                // Check for Acknowledgment; 0x28 is the Acknowlegment for Data Transfer
}

void I2C_STOP()
{												   // TWSTO: TWI Stop condition bit
	TWCR = (1<<TWINT) |(1<<TWEN) |(1<<TWSTO);      // Transmit Stop Condition
	while(TWCR & (1<<TWSTO));                      // Wait till stop condition is transmitted
}

int main(void)
{
	DDRB &=~ (1<<PINB2);
	DDRB |= (1<<PINB0);
	PORTB |= (1<<PINB2);
	
	I2C_MSTR_INIT();
	
	int flag;
	
	while (1)
	{
		
		I2C_START();
		I2C_ADDR(0x20);
		
		flag = (PINB & (1<<PINB2));
		
		if(flag)
		{
			I2C_WRITE(0b00011111);
			
		}
		else
		{
			I2C_WRITE(0b11000000);
		}
		
		I2C_STOP();
	}
}


// SLAVE


/*
 * TWI_Slave_Uni.c
 *
 * Created: 13-08-2021 23:53:49
 * Author : Kanishk Kaushal
 */ 

#include <avr/io.h>
#define F_CPU 1600000UL
#include <util/delay.h>

void blink()
{
		PORTB |=(1<<PINB0);
		_delay_ms(500);
		PORTB &=~(1<<PINB0);
		_delay_ms(500);
			
}

void I2C_SLAVE_INIT(unsigned char Slave_Addr)
{
	TWAR = Slave_Addr;                           // TWAR: TWI ADDRESS REGISTER
}

void I2C_READ()
{
	int blink_data;
	
	TWCR = (1<<TWINT) |(1<<TWEA) |(1<<TWEN);     // TWEA: TWI Enable Acknowledge Bit
	while(!(TWCR & (1<<TWINT)));			    
	while((TWSR & 0xF8) != 0x80);                // Check for Acknowledgment; 0x28 is the Acknowlegment for Data Transfer
	blink_data = TWDR;
	if(blink_data == 0b00011111)
	{
		blink(1);
	}
		
}

void I2C_ACK()
{
	while((TWSR & 0xF8) != 0x60)                  // Loop till correct acknowledgment have been received
	{
		TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWEA);
		while(!(TWCR &(1<<TWINT)));
	}
}

int main()
{
	DDRB |=(1<<PINB0);
	
	I2C_SLAVE_INIT(0x20);
	
	while(1)
	{
		I2C_ACK();
		I2C_READ();
	}
}
    

