#include <tcclib.h>                                                             
#include "avrio.h"

int main()
{
	unsigned char outval;

	//basically calls iopl(3), so we can write to our AVR IO Port range.
	setupio();

	//Configure DDRD as output.
	out8( DDRD, 0xff );

	//Configure ADC for continuous input from the thermometer
	out8( ADMUX, _BV(REFS1) | _BV(REFS0) | 8);
	out8( ADCSRA, _BV(ADEN) | _BV(ADSC) | _BV(ADATE) | 5 );

	while(1)
	{
		out8( PORTD, outval++ );
		printf( "%d / %d\n", in8( PIND ), in16( ADC ) );
	}

}

