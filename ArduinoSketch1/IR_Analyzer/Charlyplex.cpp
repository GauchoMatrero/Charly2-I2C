/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>

/*End of auto generated code by Atmel studio */
#include <util/delay.h>
//#include "usitwi.h"
//#include "INC/Charly/Charly.h"
#include "Charly.h"
// #include <Arduino.h>
const int Ndigits = 3;
const int Nsegments = 7;
int digits[Ndigits];
Charly Mpx ( Ndigits, Nsegments ) ;
// Display multiplexer **********************************************
char digit = 0;
void DisplayNextDigit() {
	static int Digit = 0;
	static int Segment = 0;
	//write Digit outs ( mask unused outs )
	PORTB =  (PORTB & 0b11111000)| Mpx.GetDigit(Digit);
	//write Segment outputs
	PORTD = ~Mpx.GetSegment(digits[Digit], Segment, Digit);
	Segment++;
	if (Segment >= Nsegments) {
		Segment = 0;
		Digit++;
		if ( Digit >= Ndigits )Digit = 0;
	}
}


// Timer interrupt - multiplexes display
ISR (TIMER1_COMPA_vect) {
	DisplayNextDigit();
}

void setup() {
	// Set up Timer1 to multiplex the display
	TCCR1A = 0 << WGM10;
	TCCR1B = 1 << WGM12 | 1 << CS10; // Divide by 1
	OCR1A = 999;                 // Compare match at 500Hz
	TIMSK |= 1 << OCIE1A;         // Compare match interrupt enable
	// Set PB0 to PB2 as outputs
	DDRB =   (DDRB  & 0b11111000)| 0x07;
	// Set PD0 to PD7 as outputs
	DDRD = 0xff;
}

void loop() {
	for (int counter = 0; counter < 1000; counter++) {
		digits [2] = counter / 100;
		digits [1] = (counter / 10) % 10;
		digits [0] = counter % 10;
		_delay_ms(500);
//		DisplayNextDigit();
	}
}

