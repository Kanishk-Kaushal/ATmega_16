/*
 * Daisy_Chain_MASTER.c
 *
 * Created: 9-07-2021 01:31:47
 * Author : Kanishk Kaushal
 */

#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>

// DEFINE SPI PINS

#define SS PINB4
#define MOSI PINB5
#define MISO PINB6
#define SCK PINB7

void Master_SPI_Init()
{
	// MOSI SCK SS -> OUTPUT
	DDRB |=(1<<MOSI) | (1<<SCK) | (1<<SS);
	
	// MISO -> INPUT
	DDRB &= ~(1<<MISO);
	
	// ENABLE SPI | MASTER MODE | FREQ/16
	SPCR |=(1<<SPE) | (1<<MSTR) | (1<<SPR0);
}

char SPI_Receive(void)
{
	// WAIT TILL ALL DATA HAS BEEN RECIEVED
	while(!(SPSR & (1<<SPIF)));     
	return(SPDR);
	
	
}

void SPI_Transmit(char data)
{
	SPDR = data;    
	
	// WAIT TILL ALL DATA HAS BEEN TRANSMITTED 
	while(!(SPSR & (1<<SPIF)));     
}

void Blink(int n)
{
	for(int i = 1;i<=n;i++)
	{
		PORTB |= (1<<PINB0);
		_delay_ms(100);
		
		PORTB &= ~(1<<PINB0);
		_delay_ms(100);
		
	}
}

int main(void)
{
	// SWITCH -> SLAVE1
	DDRB &=~(1<<PINB2);     
	PORTB |=(1<<PINB2);
	
	// SWITCH -> SLAVE2
	DDRA &=~(1<<PINA2);     
	PORTA |=(1<<PINA2);
	
	// LED
	DDRC |=(1<<PINC0); 
	
	Master_SPI_Init();
	
	int RX = 0;
	
	while (1)
	{
		// TRANSMIT:
		
		if(PINB & 1<<PINB2)
		{
			Blink(1);
			
			// SLAVE1
			SPI_Transmit(0b11110000);     
			
		}
		
		if(PINA & 1<<PINA2)
		{
			Blink(1);
			
			// SLAVE2
			SPI_Transmit(0b10100000);     
			
		}
	
		// RECIEVE
		
		RX = SPI_Receive();
		
		if (RX == 0b00001111)
		{
			Blink(3);
		}
		
	}
}



/*
 * SLAVE1.c
 *
 * Created: 8-07-2021 20:19:25
 * Author : Kanishk Kaushal
 */ 


#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>

// DEFINE SPI PINS

#define SS PINB4
#define MOSI PINB5
#define MISO PINB6
#define SCK PINB7

void SPI_Slave_Init()
{
	// MISO -> OUTPUT
	DDRB |= (1<<MISO);
	
	// ENABLE SPI | SLAVE MODE | FREQ/16
	SPCR |= (1<<SPE) | (1<<SPR0);
}

char SPI_Receive(void)
{
	// WAIT TILL ALL DATA HAS BEEN RECIEVED
	while(!(SPSR & (1<<SPIF)));     
	return(SPDR);     
}

void SPI_Transmit(char data)
{
	SPDR = data;   
	
	// WAIT TILL ALL DATA HAS BEEN TRANSMITTED  
	while(!(SPSR & (1<<SPIF)));     
}

void Blink(int n)
{
	for(int i = 1;i<=n;i++)
	{
		PORTB |= (1<<PINB0);
		_delay_ms(100);
		
		PORTB &= ~(1<<PINB0);
		_delay_ms(100);
		
	}
}

int main(void)
{
	// SWITCH -> MASTER
	DDRB &=~(1<<PINB2);     
	PORTB |=(1<<PINB2);
	
	// LED
	DDRB |=1<<PINB0;     
	
	SPI_Slave_Init();
	
	int RX = 0;
	
	while(1)
	{
		// TRANSMIT :
		
		if(PINB & 1<<PINB2)
		{
			Blink(1);			
			SPI_Transmit(0b00001111);
			
		}
		
		// RECIEVE :
		
		RX = SPI_Receive();
		
		if(RX == 0b11110000)
		{
			Blink(3);
		}
			
		else if(RX == 0b10100000)
		{
			SPI_Transmit(RX);
			
		}
	}
}


/*
 * SLAVE2.c
 *
 * Created: 9-07-2021 01:47:16
 * Author : Kanishk Kaushal
 */

#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>

// DEFINE SPI PINS

#define SS PINB4
#define MOSI PINB5
#define MISO PINB6
#define SCK PINB7

void SPI_Slave_Init()
{
	// MISO -> OUTPUT
	DDRB |= (1<<MISO);
	
	// ENABLE SPI | SLAVE MODE | FREQ/16
	SPCR |= (1<<SPE) | (1<<SPR0);
}
char SPI_Receive(void)
{
	// WAIT TILL ALL DATA HAS BEEN RECIEVED
	while(!(SPSR & (1<<SPIF)));     
	return(SPDR);
}
void SPI_Transmit(char data)
{
	SPDR = data;     
	
	// WAIT TILL ALL DATA HAS BEEN TRANSMITTED
	while(!(SPSR & (1<<SPIF)));
}

void Blink(int n)
{
	for(int i = 1;i<=n;i++)
	{
		PORTB |= (1<<PINB0);
		_delay_ms(100);
		
		PORTB &= ~(1<<PINB0);
		_delay_ms(100);
		
	}
}


int main(void)
{
	// SWITCH -> MASTER
	DDRB &=~(1<<PINB2);
	PORTB |=(1<<PINB2);
	
	// LED
	DDRB |=1<<PINB0;
	
	SPI_Slave_Init();
	
	int RX = 0;
	
	while(1)
	{
		// TRANSMIT :
		
		
		if(PINB & 1<<PINB2)
		{
			Blink(1);
			SPI_Transmit(0b00001111);
			
		}
		
		// RECIEVE :
		
		RX = SPI_Receive();
		
		if(RX == 0b10100000)
		{
			Blink(3);
		}
			
		else if(RX == 0b00001111)
		{
			SPI_Transmit(RX);
			
		}
	}
}
		if(RX == 0b10100000)
		{
			Blink(3);
		}
			
		else if(RX == 0b00001111)
		{
			SPI_Transmit(RX);
			
		}
	}
}
