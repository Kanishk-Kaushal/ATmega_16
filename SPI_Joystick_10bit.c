// MASTER

/*
 * SPI_Joystick_10bit_MASTER.c
 *
 * Created: 01-07-2021 00:03:53
 * Author : KANISHK KAUSHAL
 */ 

#define  F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t X_first,X_sec,Y_first,Y_sec;
uint16_t Tenbit;
uint8_t First_value,Sec_value,joyval,L;
uint8_t data,rec;
int i,flag=0;

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
	// ADC INITIALISE
	DDRA = 0b00000000;
	ADMUX = (1<<REFS0);
	
	// PRESCLAR = 0
	ADCSRA = (1<<ADEN);
	
	// LEFT ADJUSTED MANNER
	ADMUX |= (1<<ADLAR);
	
	// ADC INTERRUPT ENABLE
	ADCSRA |=1<<ADIE;
	
	// START OF CONVERSION
	ADCSRA |=1<<ADSC;
	
	// GLOBAL INTERRUPT ENABLE
	sei();
}

int main(void)
{
	ADC_Init();
	Master_SPI_Init();
	
	while (1)
	{
	}
}

ISR(ADC_vect)
{
	L = ADCL;
	Tenbit = (ADCH<<2 | L>>6);
	
	joyval = Tenbit;			  // SCALE DOWN TO 8 BIT
	First_value=joyval<<2;        // 6 BITS ------00
	joyval=Tenbit>>6;			  // 4 BITS 0000----
	Sec_value=joyval<<2;          // 4 BITS 00----00
	
	switch(ADMUX)
	{
		case 0x60:
		
		X_first=(First_value | 0b00000000); // 6 BITS
		X_sec=(Sec_value | 0b00000001); // 4 BITS
		
		Master_Transmit(X_first);
		Master_Transmit(X_sec);
		
		ADMUX = 0x61; // CHECK OTHER CHANNEL
		
		break;
		
		case 0x61:
		
		Y_first=(First_value | 0b00000010); // 6 BITS
		Y_sec=(Sec_value | 0b00000011); // 4 BITS
		
		Master_Transmit(Y_first);
		Master_Transmit(Y_sec);
		
		ADMUX = 0x60; // CHECK OTHER CHANNEL
		
		break;
	}
	
	ADCSRA |= 1<<ADSC;
}


// SLAVE


/*
 * SPI_Joystick_10bit_SLAVE.c
 *
 * Created: 01-07-2021 00:50:48
 * Author : KANISHK KAUSHAL
 */ 



#define  F_CPU 1000000UL
#include <avr/io.h>


uint8_t RX, X_first, X_sec, Y_first, Y_sec;
uint16_t X,Y;
int Scaled_X,Scaled_Y;

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

uint8_t Slave_Recieve(void)
{
	// WAIT FOR RECFIEVED DATA TO BE LOADED
	while(!(SPSR & (1<<SPIF)));
	
	return SPDR;
}

void Timers_Init()
{
	// TIMER 1A
	TCCR1A |= (1<<WGM10)|(1<<WGM11)|(1<<COM1A1)|(1<<COM1B1);
	
	// TIMER 1B
	TCCR1B |= (1<<WGM12)|(1<<CS10);
}

int main(void)
{
	Timers_Init();
	
	DDRD |= (1<<PIND4); // X AXIS
	DDRD |= (1<<PIND5); // Y AXIS
	
	
	Slave_SPI_Init();
	
	while (1)
	{
		RX = Slave_Recieve();
		
		if((RX & 0b00000011) == 0b00000000)
		{
			X_first = RX;
			X_first = X_first>>2;
		}
		else if((RX & 0b00000011) == 0b00000001)
		{
			X_sec = RX;
			X_sec = X_sec>>2;
		}
		else if((RX & 0b00000011) == 0b00000010)
		{
			Y_first = RX;
			Y_first = Y_first>>2;
		}
		else if((RX & 0b00000011) == 0b00000011)
		{
			Y_sec = RX;
			Y_sec = Y_sec>>2;
		}
		
		X = X_sec;
		X = X<<6 | X_first;
		Y = Y_sec;
		Y = Y<<6 | Y_first;

		Scaled_X = ((X*2)-1023);
		Scaled_Y = ((Y*2)-1023);
		
		OCR1B = (Scaled_X); // X AXIS
		OCR1A = (Scaled_Y); // Y AXIS
		
	}
}

