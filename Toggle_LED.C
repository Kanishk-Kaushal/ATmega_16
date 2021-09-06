#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

unsigned char debounce()
{
	if((PINB & (1<<PINB0)))
	{
		_delay_ms(30);
		
		if((PINB & (1<<PINB0)))
		{
			return 1;
		}
	}
	return 0;
}
int main(void)
{
	unsigned char ctbutton=0;
	unsigned char lbutton=0;

	DDRA|=(1<<PINA0); // output pin
	DDRB&=~(1<<PINB0);//input pin
	
	while (1)
	{
		ctbutton=debounce();
		if((ctbutton==1) & (lbutton==0))
		{
			PORTA^=(1<<PORTA0);
		}
		lbutton=ctbutton;

	}
}
