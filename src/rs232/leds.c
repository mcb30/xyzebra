/**
 * @file
 *
 * Status LEDs
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "leds.h"

/* Red LED is attached to PB1 */
#define RED PB1
#define PORT_RED PORTB
#define DDR_RED DDRB

/* Yellow LED is attached to PB2 */
#define YELLOW PB2
#define PORT_YELLOW PORTB
#define DDR_YELLOW DDRB

/* Green LED is attached to PC5 */
#define GREEN PC5
#define PORT_GREEN PORTC
#define DDR_GREEN DDRC

/** Flashing LED speed */
#define FLASH_HZ 4

/** CNC activity flag */
uint8_t cnc_activity;

/** RS232 activity counter */
uint8_t rs232_activity_count;

/** Error flag */
uint8_t error;

/**
 * Set red LED state
 *
 * @v state		LED state
 */
static inline __attribute__ (( always_inline )) void
leds_red ( uint8_t state ) {
	if ( state ) {
		PORT_RED = ( PORT_RED | _BV ( RED ) );
	} else {
		PORT_RED = ( PORT_RED & ~_BV ( RED ) );
	}
}

/**
 * Set yellow LED state
 *
 * @v state		LED state
 */
static inline __attribute__ (( always_inline )) void
leds_yellow ( uint8_t state ) {
	if ( state ) {
		PORT_YELLOW = ( PORT_YELLOW | _BV ( YELLOW ) );
	} else {
		PORT_YELLOW = ( PORT_YELLOW & ~_BV ( YELLOW ) );
	}
}

/**
 * Set green LED state
 *
 * @v state		LED state
 */
static inline __attribute__ (( always_inline )) void
leds_green ( uint8_t state ) {
	if ( state ) {
		PORT_GREEN = ( PORT_GREEN | _BV ( GREEN ) );
	} else {
		PORT_GREEN = ( PORT_GREEN & ~_BV ( GREEN ) );
	}
}

/**
 * Update LEDs
 *
 */
static void leds_update ( void ) {
	static uint8_t flash_phase;

	/* Invert flash phase */
	flash_phase = ( ~ flash_phase );

	/* Red: flashing indicates an error */
	if ( error ) {
		leds_red ( flash_phase );
	} else {
		leds_red ( 0 );
	}

	/* Yellow: flashing indicates recent RS232 activity */
	if ( rs232_activity_count > 0 ) {
		rs232_activity_count--;
		leds_yellow ( flash_phase );
	} else {
		leds_yellow ( 0 );
	}

	/* Green: flashing indicates CNC activity, steady indicates
	 * idle with no error
	 */
	if ( cnc_activity ) {
		leds_green ( flash_phase );
	} else {
		leds_green ( ! error );
	}
}

/**
 * Timer comparison ISR
 *
 */
ISR ( TIMER0_COMPA_vect ) {

	/* Update LEDs */
	leds_update();
}

/**
 * Initialise LEDs
 *
 */
void leds_init ( void ) {

	/* Configure LED output pins */
	DDR_RED |= _BV ( RED );
	DDR_YELLOW |= _BV ( YELLOW );
	DDR_GREEN |= _BV ( GREEN );

	/* Configure timer 0 to run in CTC mode and generate an
	 * interrupt at the specified frequency.
	 */
	TCCR0A = _BV ( WGM01 );
	OCR0A = ( ( F_CPU / 1024 ) / FLASH_HZ );
	TIMSK0 = _BV ( OCIE0A );
	TCCR0B = ( _BV ( CS02 ) | _BV ( CS00 ) );
}
