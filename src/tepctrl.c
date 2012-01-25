#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define TIMER_THRESHOLD 0x200

#define BUTTON_PINS PINC
#define BUTTON_FORWARD PC0
#define BUTTON_LEFT PC1
#define BUTTON_RIGHT PC2
#define BUTTON_DRILL PC3
#define BUTTON_BACKWARD PC4
#define SOLENOID PB0
#define MOTOR_X0 PD7
#define MOTOR_X1 PD5
#define MOTOR_X2 PD3
#define MOTOR_X3 PD1
#define MOTOR_Y0 PD6
#define MOTOR_Y1 PD4
#define MOTOR_Y2 PD2
#define MOTOR_Y3 PD0
#define DRILL_PORT PORTB
#define BUTTON_PORT PORTC
#define MOTOR_PORT PORTD
#define DRILL_SET DDRB
#define MOTOR_SET DDRD

static const uint8_t x_outputs[] = {
	( _BV ( MOTOR_X0 ) | _BV ( MOTOR_X1 ) ),
	( _BV ( MOTOR_X1 ) | _BV ( MOTOR_X2 ) ),
	( _BV ( MOTOR_X2 ) | _BV ( MOTOR_X3 ) ),
	( _BV ( MOTOR_X3 ) | _BV ( MOTOR_X0 ) ),
};

static const uint8_t y_outputs[] = {
	( _BV ( MOTOR_Y0 ) | _BV ( MOTOR_Y1 ) ),
	( _BV ( MOTOR_Y1 ) | _BV ( MOTOR_Y2 ) ),
	( _BV ( MOTOR_Y2 ) | _BV ( MOTOR_Y3 ) ),
	( _BV ( MOTOR_Y3 ) | _BV ( MOTOR_Y0 ) ),
};

ISR ( TIMER1_COMPA_vect ) {
	static uint8_t x_position;
	static uint8_t y_position;
	uint8_t buttons;

	/* Read buttons and update position */
	buttons = BUTTON_PINS;

	if ( ! ( buttons & _BV ( BUTTON_RIGHT ) ) )
		x_position++;
	if ( ! ( buttons & _BV ( BUTTON_LEFT ) ) )
		x_position--;

	if ( ! ( buttons & _BV ( BUTTON_FORWARD ) ) )
		y_position++;
	if ( ! ( buttons & _BV ( BUTTON_BACKWARD ) ) )
		y_position--;

	/* Set outputs according to current position */
	MOTOR_PORT = ( x_outputs [ x_position % sizeof ( x_outputs ) ] |
		       y_outputs [ y_position % sizeof ( y_outputs ) ] );

	if ( ! ( buttons & _BV ( BUTTON_DRILL ) ) )
		DRILL_PORT = _BV ( SOLENOID );
}

int main ( void ) {

	/* Configure input pins with internal pull-up resistors */
	BUTTON_PORT = ( _BV ( BUTTON_LEFT ) | _BV ( BUTTON_RIGHT ) |
			_BV ( BUTTON_FORWARD ) | _BV ( BUTTON_BACKWARD ) |
			_BV ( BUTTON_DRILL) );

	/* Configure output pins */
	DRILL_SET = ( _BV ( SOLENOID ) );

	MOTOR_SET = ( _BV ( MOTOR_X0 ) | _BV ( MOTOR_X1 ) | _BV ( MOTOR_X2 ) |
		     _BV ( MOTOR_X3 ) | _BV ( MOTOR_Y0 ) | _BV ( MOTOR_Y1 ) |
		     _BV ( MOTOR_Y2 ) | _BV ( MOTOR_Y3 ) );

	/* Configure timer */
	OCR1A = TIMER_THRESHOLD;
	TCCR1B = ( _BV ( WGM12 ) | _BV ( CS11 ) );
	TIMSK1 = _BV ( OCIE1A );

	/* Enable interrupts */
	sei();

	/* Sleep forever */
	while ( 1 )
		sleep_mode();
}
