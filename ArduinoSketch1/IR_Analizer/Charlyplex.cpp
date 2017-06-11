/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>

/*End of auto generated code by Atmel studio */
/***********************************************************************+/
 | ATTiny2313 setup diagram	                                            |
 |																		|
 |				  ----------					     					|
 |		Vcc  ----|1		  20|----Vcc				    				|
 |	    Seg g  --|2		  19|----SCL									|
 |		Seg f  --|3		  18|----NC										|
 |	    Seg Dp --|4		  17|----SDA									|
 |		Seg e  --|5		  16|----NC										|
 |		Seg e  --|6		  15|----NC										|
 |		Seg d  --|7		  14|---- D2									|
 |		Seg c ---|8		  13|---- D1 									|
 |		Seg b ---|9		  12|---- D0					 	            |
 |		GND -----|10	  11|---- Seg a					    			|
 |				 -----------											|
 |																		|
 ************************************************************************/

//	Default clock speed
/*
 *	The internal oscillator of the ATTiny2313 runs at 8 MHz.  If the CKDIV8
 *	fuse is set, the system clock is prescaled by 8; therefore, we are setting
 *	the F_CPU at 1 MHz
 */
//#define F_CPU 1000000UL
//#define DEBUG 0
#define I2C_ADDDR 2
#include <util/delay.h>
//#include "INC/Charly/Charly.h"
#include <avr/sfr_defs.h>
#include <compat/twi.h>
#include <avr/interrupt.h>
// Prototype for calling a C library functions
extern "C" {
	#include "usiTwiSlave.h"
}




#define NOP asm("nop");				//	skip one clock cycle

#ifndef SDA
#define SDA             (1<<PB5)    //  I2C SDA
#endif

#ifndef SCL
#define SCL             (1<<PB7)    //  I2C SCL
#endif

#define I2C_OK 	0x10	        //	stage 1 of the sequence
#define I2C_ACKNOWLEDGE	0x7F	//	acknowledgment sent back to host after successfully confirming burn
/**********************  CHARLYPLEX DEFINITIONS *************************/
#include "Charly.h"
#include "usiTwiSlave.h"
//todo: use method in charly class
// #include <Arduino.h>

// define storage used for buffering data
const int Ndigits = 3;
const int Nsegments = 7;
int digits[Ndigits+1];

// define multiplexer
Charly Mpx ( Ndigits, Nsegments ) ;
// Display multiplexer **********************************************
char digit = 0;
void DisplayNextDigit() {
	static int Digit = 0;
	static int Segment = 0;
	static int Dp =false;
	
	//write Digit outs ( mask unused outs )
	PORTB =  (PORTB & 0b11111000)| Mpx.GetDigit(Digit);
	// Check Digit Point or segment processing
	if (Dp==false)
	{
		// turn off previous Dps
		PORTA |=  (1 << PA1 )   ;  // turn on dp on digit
		//write Segment outputs
		PORTD = ~Mpx.GetSegment(digits[Digit], Segment, Digit);
		Segment++;
		if (Segment >= Nsegments) {
				Dp=true;

		}
	} 
	else
	{
			// Check & send dp segment ( as stated in MSB ... dp2 dp1 dp0)
			if ( digits[3] & ( 1 << Digit	))
			PORTA &= (~ (1<<PA1))   ; // turn on dp on digit
			// process segments on next interrupt
			Dp=false;
			// reset digit counter stuff
			Segment = 0;
			Digit++;
			// Last Segment ? restart digit count & set Dp processing
			if ( Digit >= Ndigits )
			{
				Digit = 0;
			}
			
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

	// Set PA1 as output
	DDRA |=  1 << PA1;
	// Set PB0 to PB2 as outputs
	DDRB =   (DDRB  & 0b11111000)| 0x07;
	// Set PD0 to PD7 as outputs
	DDRD = 0xff;
	//	Set  I2C address
	uint8_t slave_address = I2C_ADDDR;
	// initialize as slave with our hardware address
	usiTwiSlaveInit(slave_address);
		// enable interrupts (must be there, i2c needs them!)
	sei();

}

void loop() {

static int i=0;

			  if( usiTwiDataInReceiveBuffer() )
			  {
				  digits [i++]= usiTwiReceiveByte();
//				  digits [i++]= i;
			  }
              if (i> (Ndigits))
			       {
			// empty buffer		   
			    while( usiTwiDataInReceiveBuffer() )  usiTwiReceiveByte();
				 i=0;
				 _delay_ms(10);
			       }
			  
			  //	waste a cycle
			  //NOP

            }

