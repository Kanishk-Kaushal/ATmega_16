#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB |= (1<<DDB0); //PB0 SET AS OUTPUT
	
	DDRC &= (~(1<<DDC0)); // PC0 CLEARED IE INPUT
	
	
	
	
	while (1)
	{
		if(PINC & (1<<PINC0)) // IF PC0 IS 1
		{
			_delay_ms(100);
			PORTB |= (1<<PINB0); // PB0 IS 1 IE LED IS ON
		}
		else
		{
			_delay_ms(100);
			PORTB &= (~(1<<PINB0)); // ELSE PB0 IS 0 IE LED IS OFF
		}
		

		
		
	}
}
