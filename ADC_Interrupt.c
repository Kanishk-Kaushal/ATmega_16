#include <avr/io.h>
#include <avr/interrupt.h>

int main(void)
{
	// FAST PWM 8-BIT
	TCCR0 |= 1<<WGM00 | 1<<WGM01 ;
	
	// NON INVERTING MODE
	TCCR0 |= 1<<COM01 ;
	TCCR0 &= ~(1<COM00);
	
	// PRESCALER = 1
	TCCR0 |= 1<<CS00; 
	TCCR0 &= ~(1<<CS01) & ~(1<<CS02);
	
        //OUTPUT
	DDRB |= 1<<PINB3;
	
	//ADC INITIALISE 
	ADMUX |= 1<<ADLAR; 
	ADCSRA |= 1<<ADIE; 
	ADMUX |= 1<<REFS0; 
	ADCSRA |= 1<<ADEN; 
	sei(); // GLOBAL INTERRUPT ENABLE
	ADCSRA |= 1<<ADSC; // ADC CONERSION
	
	while (1)
	{
	}
}

ISR(ADC_vect) 
{
	uint8_t res= ADCH; 
	OCR0 = res; 
	ADCSRA |= 1<<ADSC; 
}
