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

int8_t manual_x;
int8_t manual_y;
uint8_t manual_drill;

/**
 * Handle button presses
 *
 * @v buttons		New button state
 */
static void buttons_pressed ( uint8_t buttons ) {
	static uint8_t clear_error = 0;
	uint8_t temp;
	
	/* clear error */
	if ( error ) {
		if ( ! ( buttons & _BV ( DONE ) ) ) {
			clear_error = 1;
		} else {
			if ( clear_error == 1 ) {
				clear_error = 0;
				error = 0;
			}
		}
	}

	/* Set manual_drill=1 iff D button is pressed */
	if ( ! ( buttons & _BV ( DONE ) ) ) {
		manual_drill = 1;
	} else {
		manual_drill = 0;
	}

	/* Set manual_y to +1, 0 or -1 based in F and B buttons */
	temp = 0;
	if ( ! ( buttons & _BV ( FORWARD ) ) ) {
		temp += 1;
	}
	if ( ! ( buttons & _BV ( BACKWARD ) ) ) {
		temp -= 1;
	}
	manual_y = temp;

	
	/* Set manual_x to +1, 0 or -1 based in L and R buttons */ 
	temp = 0;
	if ( ! ( buttons & _BV ( LEFT ) ) ) {
		temp -= 1;
	}
	if ( ! ( buttons & _BV ( RIGHT ) ) ) {
		temp += 1;
	}
	manual_x = temp;
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
