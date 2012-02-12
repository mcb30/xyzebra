/**
 * @file
 *
 * RS232 serial communication
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/setbaud.h>
#include "leds.h"
#include "targets.h"
#include "serial.h"

enum state { STATE_NONE, STATE_X, STATE_Y };

static enum state state;
static unsigned int X_target;
static unsigned int Y_target;

/* When called, reset state and co-ordinate values */
static void reset ( void ) {
	state = STATE_NONE;
	Y_target = 0;
	X_target = 0;
}

/**
 * Handle received data
 *
 * @v data		Received character
 */
static void receive ( char data ) {
	uint8_t value;

	/* Set the RS232 activity counter */
	rs232_activity_count = 5;

	/* If there is an error end process */
	if ( error != 0 )
		return;

	/* If data is a co-ordinate number, store it */
	if ( ( data >= '0' ) && ( data <= '9' ) ) {

		/* Convert the character (data), into a number (value) */
		value = data - '0';
		
		/* If state is X, add data to X_target */
		if ( state == STATE_X ) {
			X_target *= 10;
			X_target += value;
		}

		/* If state is Y, add data to Y_target */
		else if ( state == STATE_Y ) {
				Y_target *= 10;
				Y_target += value;
		}

		/* If the state is not X or Y, set an error and reset */
		else {
			error = 1;
			reset ();
		}	
	}

	/* If data is an X, set co-ordinate letter to X */
	else if ( data == 'X' ) {
		state = STATE_X;
	}

	/* If data is an Y, set co-ordinate letter to Y */
	else if ( data == 'Y' ) {
		state = STATE_Y;
	}

	/* If data is a new line operator, store co-ordinates and reset */
	else if ( data == '\r' ) {
		targets [ target_prod ].x = X_target;
		targets [ target_prod ].y = Y_target;
		target_prod += 1;
		reset ();
	}

	/* If data is not useable, set an error and reset */
	else {
		error = 1;
		reset ();
	}

}

/**
 * RX completion ISR
 *
 */
ISR ( USART_RX_vect ) {
	char data;

	/* Pick up character from serial port */
	data = UDR0;

	UDR0 = data;

	/* Handle received data */
	receive ( data );
}

/**
 * Initialise serial port
 *
 */
void serial_init ( void ) {

	/* Configure USART0 to operate in UART mode at the specified
	 * baud rate (8 data bits, 1 stop bit, no parity).  Enable RX
	 * completion interrupts.
	 */
	UCSR0A = ( USE_2X ? _BV ( U2X0 ) : 0 );
	UCSR0B = ( _BV ( RXCIE0 ) | _BV ( RXEN0 ) | _BV ( TXEN0 ) );
	UCSR0C = ( _BV ( UCSZ01 ) | _BV ( UCSZ00 ) );
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
}
