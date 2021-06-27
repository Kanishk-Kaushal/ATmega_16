//MASTER

/*
 * SPI_Bidirectional_Master.c
 *
 * Created: 27-06-2021 21:19:25
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

void Master_SPI_init()
{
	// MOSI SCK SS -> OUTPUT
	DDRB |=(1<<MOSI) | (1<<SCK) | (1<<SS);
	
	// MISO -> INPUT      
	DDRB &= ~(1<<MISO);   
	
	// ENABLE SPI | MASTER MODE | FREQ/16
	SPCR |=(1<<SPE) | (1<<MSTR) | (1<<SPR0);      
	
}



unsigned char Bidirectional_comm(unsigned char data)
{
	// START TRANSMISSION 
	SPDR =data;
	
	// WAIT FOR TRANSMISSION TO COMPLETE
	while(!(SPSR & (1<<SPIF)));
	
	// RETURN BACK THE SAME FOR 2 WAY COMMUNICATION	
	return SPDR;
}

int main(void)
{
	Master_SPI_init();
	
	// PUSHBUTTON
	DDRB &= ~(1<<PINB0); 
	
	// LED
	DDRC=0xFF; 
	
	unsigned char RX;
	unsigned char TX;
	
	while (1)
	{
		
		
		if(PINB & (1<<PINB0)) // IF PINB0 IS SET -> PUSHBUTTON IS PRESSED
		{
			TX=0b11110000;
		}
		else
		{
			
			TX=0b00000000;
		}
		
		RX=0x00; // INITIALISING RX
		
		RX=Bidirectional_comm(TX);
		
		if (RX == 0b10100000)
		{
			PORTC |=(1<<PORTC0);
		}
		else
		{
			PORTC &=~(1<<PORTC0);
		}
		
	}
}


// SLAVE


/*
 * SPI_Bidirectional_Slave.c
 *
 * Created: 27-06-2021 21:50:40
 * Author : Kanishk Kaushal
 */ 

#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

//DEFINE SPI PINS

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


unsigned char Bidirectional_comm(unsigned char data)
{
	SPDR =data;
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}

int main(void)
{
	Slave_SPI_Init();
	
	// PUSHBUTTON
	DDRB &= ~(1<<PINB0);
	
	// LED
	DDRC=0xFF;
	
	unsigned char RX;
	unsigned char TX;
	
	while (1)
	{
		
		if(PINB & (1<<PINB0)) // IF PINB0 IS SET -> PUSHBUTTON IS PRESSED
		{
			TX=0b10100000;
		}
		else
		{
			TX=0b00000000;
		}
		
		RX=0x00; // INITALISING RX
		
		RX=Bidirectional_comm(TX);
		
		if (RX == 0b11110000)
		{
			PORTC |=(1<<PORTC0);
		}

		else
		{
			PORTC &=~(1<<PORTC0);
			
		}
		
	}
}

