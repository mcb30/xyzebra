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
#include <stdio.h>

#define MAX_DRILL_BITS 10
#define BUFFER_SIZE 128

enum state { STATE_NONE, STATE_C, STATE_G, STATE_M, STATE_T, STATE_X, STATE_Y, STATE_IGNORE };

struct drillbits {
	unsigned int diameter;
};

struct drillbits C_value[ MAX_DRILL_BITS ] = {
};		

static enum state state;
static unsigned int X_target;
static unsigned int Y_target;
static unsigned int new_X_target;
static unsigned int new_Y_target;
static unsigned int G_value;
static unsigned int M_value;
static unsigned int T_value;
static unsigned int value;
static uint8_t relative_coordinates;
static uint8_t consumer = 0;
static uint8_t producer = 0;
static char outbuff[BUFFER_SIZE];


/* When called, reset state and co-ordinate values */
static void reset ( void ) {
	state = STATE_NONE;
	new_Y_target = 0;
	new_X_target = 0;
	G_value = 0;
	M_value = 0;
	T_value = 0;
}


/* Handles data in state C */
static void handle_c ( char data ) {

	/* If data is a number, store it */
	if ( ( data >= '0' ) && ( data <= '9' ) ) {
		
		/* Convert the character (data), into a number (value) */
		value = data - '0';
		
		/* Add data to C_value */
		C_value[T_value-1].diameter *= 10;
		C_value[T_value-1].diameter += value;
	}

	else if ( data == '.' ) {
	}

	/* If data is a new line operator reset */
	else if ( data == '\r' ) {
		printf ( "Db %d", T_value );
		printf ( ": %d\n", C_value[T_value-1].diameter );
		reset ();

	} else {
		/* If data is unusable, set error */
		printf( "\nError: State C, invalid character" );
		error = 1;
		reset();
	}
}

/* Handles data in state G */
static void handle_g ( char data ) {

	/* If data is a number, store it */
	if ( ( data >= '0' ) && ( data <= '9' ) ) {
		
		/* Convert the character (data), into a number (value) */
		value = data - '0';
		
		/* Add data to G_value */
		G_value *= 10;
		G_value += value;
	}

	/* If data is a new line operator process and reset */
	else if ( data == '\r' ) {
		/* Set relative or absolute co-ordinates */
		if ( G_value == 90 ) {
			relative_coordinates = 0;
		}
		else if ( G_value == 91 ) {
			relative_coordinates = 1;
		}
		printf ( "G: %d\n",  G_value );
		reset ();

	} else {
		/* If data is unusable, set error */
		printf( "\nError: State G, invalid character" );		
		error = 1;
		reset();
	}
}

/* Handles data in state M */
static void handle_m ( char data ) {
		
	/* If data is a number, store it */
	if ( ( data >= '0' ) && ( data <= '9' ) ) {

		/* Convert the character (data), into a number (value) */
		value = data - '0';
		
		/* Add data to M_value */
		M_value *= 10;
		M_value += value;
	}

	/* If data is a new line operator reset */
	else if ( data == '\r' ) {
		printf ( "M: %d\n",  M_value );
		reset ();

	} else {
		/* If data is unusable, set error */
		printf( "\nError: State M, invalid character" );
		error = 1;
		reset();
	}
}

/* Handles data in state T */
static void handle_t ( char data ) {

	/* If data is a co-ordinate number, store it */
	if ( ( data >= '0' ) && ( data <= '9' ) ) {
		
		/* Convert the character (data), into a number (value) */
		value = data - '0';
		
		/* Add data to T_value */
		printf ( "T: %d\n", T_value );	
		T_value *= 10;
		T_value += value;
		
		/* If T_value is too large set error */
		if ( T_value > MAX_DRILL_BITS ) {
			printf( "\nError: State T, T_value is too large" );
			error = 1;
			reset();
		}
	}
	
	/* If data is a C, set state to C */
	else if ( data == 'C' ) {
		state = STATE_C;
	}

	/* If data is a new line operator reset */
	else if ( data == '\r' ) { 
		printf ( "T: %d\n",  T_value );
		reset ();

	} else {
		/* If data is unusable, set error */
		printf( "\nError: State T, invalid character" );
		error = 1;
		reset();
	}
}

/* Handles data in state X and Y */
static void handle_x_and_y ( char data ) {

	/* If data is a co-ordinate number, store it */
	if ( ( data >= '0' ) && ( data <= '9' ) ) {
		
		/* Convert the character (data), into a number (value) */
		value = data - '0';
		
		/* If state is X, add data to X_target */
		if ( state == STATE_X ) {
			new_X_target *= 10;
			new_X_target += value;
		}
		
		/* If state is Y, add data to Y_target */
		else if ( state == STATE_Y ) {
			new_Y_target *= 10;
			new_Y_target += value;
		}
	}

	else if ( data == '.' ) {
	}
	
	/* If data is Y, set state to Y */
	else if ( data == 'Y' ) {
		state = STATE_Y;
	}

	/* If data is a new line operator, store co-ordinates and reset */
	else if ( data == '\r' ) {
		/* Store co-ordinates in relative or absolute mode */
		if ( relative_coordinates == 1 ) {
			X_target += new_X_target;
			Y_target += new_Y_target;
		} else {
			X_target = new_X_target;
			Y_target = new_Y_target;			
		}
		targets [ target_prod ].x = X_target;
		targets [ target_prod ].y = Y_target;
		printf( "X: %d", new_X_target );
		printf( ", Y: %d\n", new_Y_target );
		target_prod += 1;
		reset ();
	}

	/* If data is unusable, set error */
	else {
		printf( "\nError: State X or Y, invalid character" );
		error = 1;
		reset();
	}
}

/**
 * Handle received data
 *
 * @v data		Received character
 */
static void receive ( char data ) {

	/* Set the RS232 activity counter */
	rs232_activity_count = 5;

	/* If there is an error end process */
	if ( error != 0 )
		return;

	else if ( state == STATE_NONE ) {
		
		/* If data is an G, set state to G */
		if ( data == 'G' ) {
			state = STATE_G;
		}
		
		/* If data is an M, set state to M */
		else if ( data == 'M' ) {
			state = STATE_M;
		}
		
		/* If data is an T, set state to T */
		else if ( data == 'T' ) {
			state = STATE_T;
		}
		
		/* If data is an X, set state to X */
		else if ( data == 'X' ) {
			state = STATE_X;
		}
		
		/* If data is an Y, set state to Y */
		else if ( data == 'Y' ) {
			state = STATE_Y;
			
		} else {
			state = STATE_IGNORE;
		}
	}
	
	/* If the state is C, process data */
	else if ( state == STATE_C ) {
		handle_c ( data );
	}
	
	/* If the state is M, process data */
	else if ( state == STATE_M ) {
		handle_m ( data );
	}		
	
	/* If the state is G, process data */
	else if ( state == STATE_G ) {
		handle_g ( data );
	}
	
	/* If the state is T, process data */
	else if ( state == STATE_T ) {
		handle_t ( data );
	}
	
	/* If the state is X or Y, process data */
	else if ( ( state == STATE_X ) || ( state == STATE_Y ) ) {
		handle_x_and_y ( data );

	}

	/* If data is a new line reset */
	else {	
		if ( data == '\r' )
			reset();
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

	/* Handle received data */
	receive ( data );
}


static int outbuff_is_full ( void ) {
	return ( ( consumer % BUFFER_SIZE == producer % BUFFER_SIZE ) && ( consumer != producer ) );
}

static int outbuff_is_empty ( void ) {
	return ( consumer == producer );
}

static void serial_transmit ( void ) {
	/* If outbuff is not empty - and increase consumer */
	if ( !( outbuff_is_empty() ) ) {
		UDR0 = outbuff[ consumer % BUFFER_SIZE ];
		consumer++;
		}
	/* If outbuff is empty disable the ISR */
	else {
		UCSR0B |= ! ( _BV( UDRIE0 ) );		
	}
}

ISR ( USART_UDRE_vect ) {
	serial_transmit();
}

/**
 * Write character to serial port
 *
 * @c c			Character to write
 */
static int serial_putchar ( char c, FILE *stream ) {

	/* Convert LF to CRLF */
	if ( c == '\n' )
		serial_putchar ( '\r', stream );

	/* If outbuff is not full add the character to the buffer, increase producer
	   and turn on the ISR */
	if ( !( outbuff_is_full() ) ) {
		outbuff[ producer % BUFFER_SIZE ] = c;
		producer++;
		UCSR0B |= _BV( UDRIE0 );
	}
	return 0;
}

/* Define standard output stream */
static FILE serial_stdout =
	FDEV_SETUP_STREAM ( serial_putchar, NULL, _FDEV_SETUP_WRITE );
 
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

	/* Use serial port for stdout */
	stdout = &serial_stdout;
}
