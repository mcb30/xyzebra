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

/**
 * Handle received data
 *
 * @v data		Received character
 */
static void receive ( uint8_t data ) {

	/* Placeholder code.  Remove this code and replace it with
	 * your own.
	 */

	/* Set the RS232 activity counter */
	rs232_activity_count = 2;

	/* Set/clear error flag if we receive 'X'/'x' (respectively) */
	if ( data == 'X' ) {
		error = 1;
	} else if ( data == 'x' ) {
		error = 0;
	}

	/* Set/clear CNC activity flag if we receive 'C'/'c' (respectively) */
	if ( data == 'C' ) {
		cnc_activity = 1;
	} else if ( data == 'c' ) {
		cnc_activity = 0;
	}

	/* Echo back the received character */
	UDR0 = data;

}

/**
 * RX completion ISR
 *
 */
ISR ( USART_RX_vect ) {
	uint8_t data;

	/* Pick up character from serial port */
	data = UDR0;

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
