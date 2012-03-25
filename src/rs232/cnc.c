/**
 * @file
 *
 * CNC controller
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "leds.h"
#include "targets.h"
#include "buttons.h"
#include "cnc.h"

/* Solenoid is attached to PB0 */
#define SOLENOID PB0
#define PORT_SOLENOID PORTB
#define DDR_SOLENOID DDRB

/* Stepper motor outputs are split between port B and port D */
#define X0_D PD7
#define Y0_D PD6
#define X1_D PD5
#define Y2_D PD4
#define X3_D PD3
#define Y3_D PD2
#define Y1_B PB7
#define X2_B PB6

/** Stepper motor port B mask */
#define STEPPER_MASK_B ( _BV ( Y1_B ) | _BV ( X2_B ) )

/** Stepper motor port D mask */
#define STEPPER_MASK_D ( _BV ( X0_D ) | _BV ( Y0_D ) | _BV ( X1_D ) | \
			 _BV ( Y2_D ) | _BV ( X3_D ) | _BV ( Y3_D ) )

/** Stepper motor update frequency */
#define STEPPER_HZ 250

/** A stepper motor position */
struct stepper_position {
	/** Port B outputs */
	uint8_t portb;
	/** Port D outputs */
	uint8_t portd;
};

/** X stepper motor positions */
static const struct stepper_position x_stepper[4] = {
	{	/* X0 and X2 */
		.portd = ( _BV ( X0_D ) ),
		.portb = ( _BV ( X2_B ) ),
	},
	{	/* X2 and X1 */
		.portb = ( _BV ( X2_B ) ),
		.portd = ( _BV ( X1_D ) ),
	},
	{	/* X1 and X3 */
		.portd = ( _BV ( X1_D ) |
			   _BV ( X3_D ) ),
	},
	{	/* X3 and X0 */
		.portd = ( _BV ( X3_D ) |
			   _BV ( X0_D ) ),
	},
};

/** Y stepper motor positions */
static const struct stepper_position y_stepper[4] = {
	{	/* Y0 and Y2 */
		.portd = ( _BV ( Y0_D ) |
			   _BV ( Y2_D ) ),
	},
	{	/* Y2 and Y1 */
		.portd = ( _BV ( Y2_D ) ),
		.portb = ( _BV ( Y1_B ) ),
	},
	{	/* Y1 and Y3 */
		.portb = ( _BV ( Y1_B ) ),
		.portd = ( _BV ( Y3_D ) ),
	},
	{	/* Y3 and Y0 */
		.portd = ( _BV ( Y3_D ) |
			   _BV ( Y0_D ) ),
	},
};

static signed int sign ( signed int k ) {
	if ( k > 0 ) {
		return +1;
	} else if ( k < 0 ) {
		return -1;
	} else {
		return 0;
	}
}

/**
 * Update CNC controller
 *
 */
static void cnc_update ( void ) {
	static uint16_t x_pos = 0;
	static uint16_t y_pos = 0;
	static uint16_t drill_counter;
	uint8_t stepper_b;
	uint8_t stepper_d;
	static uint16_t x_target;
	static uint16_t y_target;
	
	/* Check to see if there's an error */
	if ( error == 1 ) {
		PORT_SOLENOID |= _BV ( SOLENOID );
		return;
	}
		

	/* Update stepper motor outputs */
	x_pos = x_pos + sign (x_target - x_pos);
	y_pos = y_pos + sign (y_target - y_pos);
	stepper_b = ( x_stepper[x_pos % 4].portb | y_stepper[y_pos % 4].portb );
	stepper_d = ( x_stepper[x_pos % 4].portd | y_stepper[y_pos % 4].portd );
	PORTB = ( ( PORTB & ~STEPPER_MASK_B ) | stepper_b );
	PORTD = ( ( PORTD & ~STEPPER_MASK_D ) | stepper_d );

	/* Is position at target? */
	if  ( ( x_pos != x_target ) || ( y_pos != y_target ) ) {
		/* Solenoid down */
		PORT_SOLENOID |= _BV ( SOLENOID );
		return;
	}

	/* Is drill counter 0? */
	if ( drill_counter != 0 ) {
		drill_counter--;

		/* Solenoid up */
		PORT_SOLENOID &= ~_BV ( SOLENOID );
		
		return;
	}

	PORT_SOLENOID |= _BV ( SOLENOID );

	/* Does the produced equal the consumer? */
	if ( target_prod == target_cons ){
	        cnc_activity = 0;
		x_target = ( manual_x + x_target );
		y_target = ( manual_y + y_target );
		if ( manual_drill ) {
			PORT_SOLENOID &= ~_BV ( SOLENOID );
		} else {
			PORT_SOLENOID |= _BV ( SOLENOID );
		}
		return;
	}

	cnc_activity = 1;
	x_target = targets[target_cons].x;
	y_target = targets[target_cons].y;
	target_cons = target_cons +1;
	drill_counter = 500;
	return;

}

/**
 * Timer comparison ISR
 *
 */
ISR ( TIMER1_COMPA_vect ) {

	/* Update CNC controller */
	cnc_update();
}

/**
 * Initialise CNC controller
 *
 */
void cnc_init ( void ) {

	/* Configure CNC output pins */
	DDR_SOLENOID |= _BV ( SOLENOID );
	DDRB |= STEPPER_MASK_B;
	DDRD |= STEPPER_MASK_D;

	/* Configure timer 1 to run in CTC mode and generate an
	 * interrupt at the specified frequency.
	 */
	OCR1A = ( ( F_CPU / 8 ) / STEPPER_HZ );
	TCCR1B = ( _BV ( WGM12 ) | _BV ( CS11 ) );
	TIMSK1 = _BV ( OCIE1A );
}
