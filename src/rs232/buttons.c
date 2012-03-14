/**
 * @file
 *
 * Buttons
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "leds.h"
#include "buttons.h"

/* Buttons are all attached to port C */
#define FORWARD PC0
#define LEFT PC1
#define RIGHT PC2
#define DONE PC3
#define BACKWARD PC4
#define PORT_BUTTONS PORTC
#define PIN_BUTTONS PINC
#define DDR_BUTTONS DDRC
#define BUTTONS_MASK ( _BV ( LEFT ) | _BV ( RIGHT ) | _BV ( DONE ) | \
		       _BV ( FORWARD ) | _BV ( BACKWARD ) )

/**
 * Handle button presses
 *
 * @v buttons		New button state
 */
static void buttons_pressed ( uint8_t buttons ) {
	static uint8_t clear_error = 0;
	
	/* clear error */
	if ( error ) {
		if ( ! ( buttons &_BV ( DONE ) ) ) {
			clear_error = 1;
		} else {
			if ( clear_error == 1 ) {
				clear_error = 0;
				error = 0;
			}
		}
	}
}

/**
 * Pin change ISR
 *
 */
ISR ( PCINT1_vect ) {
	uint8_t buttons;

	/* Read button state */
	buttons = PIN_BUTTONS;

	/* Handle change of button state */
	buttons_pressed ( buttons );
}

/**
 * Initialise buttons
 *
 */
void buttons_init ( void ) {

	/* Configure buttons with internal pull-up resistors */
	PORT_BUTTONS |= BUTTONS_MASK;

	/* Configure buttons to trigger pin change interrupt */
	PCMSK1 = BUTTONS_MASK;
	PCICR = _BV ( PCIE1 );
}
