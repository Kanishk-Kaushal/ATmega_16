//MASTER


/*
 * SPI_Master_ADC_10bit.c
 *
 * Created: 28-06-2021 11:40:10
 * Author : KANISHK KAUSHAL
 */ 

#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
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

void ADC_Init()
{
	//ADC INITIALISE
	DDRA = 0b00000000;
	ADMUX = (1<<REFS0);
	
	//PRESCLAR = 8
	ADCSRA = (1<<ADEN)|(1<<ADPS1) |(1<<ADPS0);
	
	ADMUX &=~(1<<ADLAR);
	ADCSRA |=1<<ADIE;
	
	//START OF CONVERSION
	ADCSRA |=1<<ADSC;
	
	//GLOBAL INTERRUPT ENABLE
	sei();
}

int main(void)
{
	Master_SPI_Init();
	ADC_Init();
	
	
	
	while (1)
	{
		
	}
}

ISR(ADC_vect)
{
	    	uint8_t L = ADCL; // - - - - - - L L
		uint16_t H = ADCH<<8 | L; // H H H H H H H H L L - 10 BIT VALUE
		
		int x_first=0,y_first=0,x_sec=0,y_sec=0;
		
		switch (ADMUX)
		{
			case 0b01000000:
			
			x_first = (H | 0b00000001);
			x_sec = H>>8;
			Master_Transmit(x_first);
			Master_Transmit(x_sec);
			ADMUX = 0b01000001; //CHECK OTHER CHANNEL
			break;
			
			case 0b01000001:
			y_first = (H & 0b11111110);
			y_sec = H>>8;
			Master_Transmit(y_first);
			Master_Transmit(y_sec);
			ADMUX = 0b01000000; //CHECK OTHER CHANNEL
			break;
		}
		
		ADCSRA |= 1<<ADSC; 
}





/*
 * SPI_Slave_ADC_10bit.c
 *
 * Created: 28-06-2021 12:47:47
 * Author : KANISHK KAUSHAL
 */ 

#include <avr/io.h>

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

void Timers_init()
{
	// TIMER 0  | 8 BIT FAST PWM | NON-INVERTING  | O PRESCALER
	TCCR0 |= (1<<WGM00) |(1<<WGM01);
	TCCR0 |= (1<<COM01) |(1<<CS00);
	
	// TIMER 2  | 8 BIT FAST PWM | NON-INVERTING   | O PRESCALER
	TCCR2 |= (1<<WGM20) |(1<<WGM21);
	TCCR2 |= (1<<COM21) |(1<<CS20);
	
}

unsigned char Slave_Recieve(void)
{
	//WAIT FOR RECFIEVED DATA TO BE LOADED
	while(!(SPSR & (1<<SPIF)));
	
	return SPDR;
}

//MAP FUNCTION SOURCE CODE
long map(long x, long in_min, long in_max, long out_min, long out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main(void)
{
	Slave_SPI_Init();
	
	Timers_init() ;
	
	// LEDs
	DDRD |= (1<<PIND7);
	DDRB |= (1<<PINB3);
	
	PORTD &= ~(1<<PIND7);
	PORTB &= ~(1<<PINB3);
	
	uint16_t Rx=0;
	int L=0,H=0;
	int x=0,y=0;
	int x_map,y_map;
	
	while (1)
	{
		
		L = Slave_Recieve();
		H = Slave_Recieve();
		
		Rx = H<<8 | L;
		
		if((Rx & 0b00000001) == 0b00000001) // CHECKS WETHER RECIEVED VALUE IS FOR X AXIS
		{
			
			x = Rx;
		}
		else if((Rx & 0b00000000) == 0b00000000) // CHECKS WETHER RECIEVED VALUE IS FOR Y AXIS
		{
			
			y = Rx;
		}
		
		x_map = map(x,0,1023,0,255);
		y_map = map(y,0,1023,0,255);
		
		OCR2 = x_map;
		OCR0 = y_map;
		
		
		
	}
}

