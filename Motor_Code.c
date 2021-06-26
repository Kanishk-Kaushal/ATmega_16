/*
 * Motor_Code_Atmega16.c
 *
 * Created: 05-06-2021 08:45:00
 * Author : Kanishk Kaushal
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

//MAP FUNCTION SOURCE CODE
long map(long x, long in_min, long in_max, long out_min, long out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int x,y;
int a,b;
int PR,PL;
int low_val,tenbit_val,joyval;
int pwm_R;
int pwm_L;
int y_map;
int x_map;



int main(void)
{
    /* Replace with your application code */
	
	// TIMER 0  | 8 BIT FAST PWM | NON-INVERTING  | O PRESCALER	
	TCCR0 |= 1<<WGM00 | 1<<WGM01 | 1<<COM01 | 1<<CS00; 
	
	// TIMER 2  | 8 BIT FAST PWM | NON-INVERTING   | O PRESCALER
	TCCR2 |= 1<<WGM20 | 1<<WGM21 | 1<<COM21 | 1<<CS20;
	
	DDRB |= 1<<PINB3; // RIGHT SPEED
	DDRD |= 1<<PIND7; // LEFT SPEED
	
	DDRB |= 1<<PINB0 | 1<<PINB1;
	
	PORTB &= ~(1<<PINB0); // RIGHT DIRECTION
	PORTB &= ~(1<<PINB1); // LEFT DIRECTION
	
	// ADC INITIALISE 
	ADCSRA |= 1<<ADIE;
	ADMUX |= 1<<REFS0;
	ADMUX |= 1<<ADLAR;
	ADCSRA |= 1<<ADEN;
	sei(); // GLOBAL INTERRUPT
	ADCSRA |= 1<<ADSC; // CONVERSION
	
    while (1) 
    {
    }
}
ISR(ADC_vect)
{
	low_val=ADCL;
	tenbit_val = (ADCH<<2 | low_val>>6); // SHIFTING ADCH BITS (8) TO LEFT BY 2 & ADCL BITS (2) TO RIGHT BY 6 & "OR-ING" THEM TO GET A 10-BIT VALUE
	joyval=tenbit_val;
	
	
	switch(ADMUX)
	{
		case 0x60:
		x=joyval;
		ADMUX = 0x61; // CHECKING FOR OTHER AXIS
		break;
		case 0x61:
		y=joyval;
		ADMUX = 0x60;
		break;
	}
	
	//BUFFER :
	if(y>500 && y<=600)
	{y=512;}
	if(x>500 && x<=650)
	{x=512;}

	// DIRECTION :
	if(y<512 && x==512)
	{
		
		PORTB |= 1<<PINB0;
		PORTB |= (1<<PINB1);
	}
	else if(y>=512 && x==512)
	{
		
		PORTB &= ~(1<<PINB0);
		PORTB &= ~(1<<PINB1);
	}
	else if(x>512)
	{
		
		PORTB &= ~(1<<PINB0);
		PORTB |= (1<<PINB1);
	}
	else
	{
		
		PORTB |= (1<<PINB0);
		PORTB &= ~(1<<PINB1);
	}

	// SPEED :
	if(y<=512 && x>=512)
	{
		y_map=map(y,512,0,0,255);
		x_map=map(x,512,1023,0,255);
		if(y_map-x_map<=0)
		{

			pwm_R=x_map-y_map;
			pwm_L=x_map+y_map;
			if(pwm_R>255)
			{pwm_R=255;}
			if(pwm_L<0)
			{pwm_L=0;}
		}
		else
		{
			pwm_R=y_map-x_map;
			pwm_L=y_map+x_map;
			if(pwm_L>255)
			{pwm_L=255;}
			if(pwm_R<0)
			{pwm_R=0;}
		}
	}
	else if(y<512 && x<512)
	{y_map=map(y,512,0,0,255);
		x_map=map(x,512,0,0,255);
		if(y_map-x_map>0)
		{
			pwm_R=y_map+x_map;
			pwm_L=y_map-x_map;
			if(pwm_L>255)
			{pwm_L=255;}
			if(pwm_R<0)
			{pwm_R=0;}
			
		}
		else
		{
			pwm_R=x_map+y_map;
			pwm_L=x_map-y_map;
			if(pwm_R>255)
			{pwm_R=255;}
			if(pwm_L<0)
			{pwm_L=0;}
			
		}
	}
	else if(y>512 && x<512)
	{
		y_map=map(y,512,1023,0,255);
		x_map=map(x,512,0,0,255);
		if(y_map-x_map<0)
		{
			pwm_R=x_map-y_map;
			pwm_L=x_map+y_map;
			if(pwm_L>255)
			{pwm_L=255;}
			if(pwm_R<0)
			{pwm_R=0;}
			
		}
		else
		{
			pwm_R=y_map-x_map;
			pwm_L=y_map+x_map;
			if(pwm_L>255)
			{pwm_L=255;}
			if(pwm_R<0)
			{pwm_R=0;}
			
		}
	}
	else
	{
		y_map=map(y,512,1023,0,255);
		x_map=map(x,512,1023,0,255);
		if(y_map-x_map>0)
		{
			pwm_R=y_map+x_map;
			pwm_L=y_map-x_map;
			if(pwm_L>255)
			{pwm_L=255;}
			if(pwm_R<0)
			{pwm_R=0;}
			
		}
		else
		{pwm_R=x_map+y_map;
			pwm_L=x_map-y_map;
			if(pwm_L>255)
			{pwm_L=255;}
			if(pwm_R<0)
			{pwm_R=0;}

		}
	}
	OCR2 = pwm_L;
	OCR0 = pwm_R;
	
	
	
	ADCSRA |= 1<<ADSC;
}

