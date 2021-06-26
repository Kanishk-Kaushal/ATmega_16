//MASTER

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

void Master_Transmit(unsigned char Tx)
{
	// START TRANSMISSION
	SPDR = Tx;
	
	// WAIT FOR TRANSMISION TO COMPLETE
	while(!(SPSR & (1<<SPIF)));
}

int main(void)
{
	Master_SPI_Init();
	
	// PUSHBUTTON 	
	DDRB &=~(1<<PINB0); 
	 
	while (1)
	{
		if(PINB & (1<<PINB0)) // IF PINB0 IS SET -> PUSHBUTTON IS PRESSED
		{
			Master_Transmit(0b11110000);
		}
		else // NOT PRESSED
		{
			Master_Transmit(0b00000000);
		}
	}
}



// SLAVE


#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

// DEFINE SPI PINS 

#define SS PINB4
#define MOSI PINB5
#define MISO PINB6
#define SCK PINB7

void Slave_SPI_Init()
{
	// MISO -> OUTPUT
	DDRB = (1<<MISO); 
	
	// ENABLE SPI | SLAVE MODE
	SPCR = (1<<SPE); 
}

int main(void)
{
	Slave_SPI_Init();
	
	// LED 
	DDRC=0xFF; 
	
	unsigned char Rx;
	while (1)
	{
		//WAIT FOR RECFIEVED DATA TO BE LOADED
		while(!(SPSR & (1<<SPIF)));
		
		Rx=SPDR;
		
		if (Rx == 0b11110000)
		{
			PORTC |= (1<<PINC0); // LED GLOWS
		}
		else
		{
			PORTC &= ~(1<<PINC0); // LED OFF
		}
			
	
	}
}
