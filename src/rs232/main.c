/**
 * @file
 *
 * Main program
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "leds.h"
#include "serial.h"

/**
 * Main program
 *
 */
int main ( void ) {

	/* Enable internal pull-up resistors on unused pins */
	PORTB = ( _BV ( PB3 ) | _BV ( PB4 ) | _BV ( PB5 ) );
	PORTC = ( _BV ( PC6 ) );

	/* Initialise LEDs */
	leds_init();

	/* Initialise serial port */
	serial_init();

	/* Enable interrupts */
	sei();

	/* Sleep forever */
	while ( 1 )
		sleep_mode();
}
