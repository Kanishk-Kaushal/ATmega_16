#include <avr/io.h>
#include <avr/interrupt.h>

int main(void)
{  
  // SET MODE OF THE TIMER
	TCCR1A |= (1<<WGM10) | (1<<WGM11);
	TCCR1B |= (1<<WGM12);
	TCCR1B &= ~(1<<WGM13);                   // FAST PWM 10 - BIT
	
	TCCR1A |= (1<<COM1A1);
	TCCR1A &= ~(1<<COM1A0);      // PWM IN NON INVERTING MODE
	
	
	// SET THE PRESCALER FOR THE TIMER FOR SETTING PWM FREQUENCY
	TCCR1B |= (1<<CS10) ;
	TCCR1B &= ~(1<<CS12) & ~ (1<<CS11);             // PRESCALER = 1
	// FREQ OF PWM = 16MHz / 1 = 16MHz
	
	// FAST PWM 8-BIT
	TCCR0 |= 1<<WGM00 | 1<<WGM01 ;
	
	// NON INVERTING MODE
	TCCR0 |= 1<<COM01 ;
	TCCR0 &= ~(1<COM00);
	
	// PRESCALER = 1
	TCCR0 |= 1<<CS00;
	TCCR0 &= ~(1<<CS01) & ~(1<<CS02);
	// LEDS
	DDRB |= 1<<PINB3;
	DDRD |= 1<<PIND5;  
  
	//ADC INITIALIZATION
	ADCSRA |= 1<<ADIE;
	ADMUX |= 1<<REFS0;
	ADMUX |= 1<<ADLAR;
	ADCSRA |= 1<<ADEN;
	sei(); // GLOBAL INTERRUPT ENALBE
	ADCSRA |= 1<<ADSC; // ADC CONVERSION
	
	while (1)
	{
	}
}

ISR(ADC_vect)
{
	uint8_t res = ADCH;
	
	switch(ADMUX)
	{
		case 0x60:
		OCR0=res;
		ADMUX = 0x61; // CHANGE OF CHANNEL
		break;
		
		case 0x61:
		OCR1A=res;
		ADMUX = 0x60;
		break;
	}
	
	ADCSRA |= 1<<ADSC; // CONVERSION
}
