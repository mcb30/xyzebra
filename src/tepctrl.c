#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define TIMER_THRESHOLD 0x4000

static const uint8_t outputs[] = {
	( _BV ( PD0 ) | _BV ( PD1 ) ),
	( _BV ( PD1 ) | _BV ( PD2 ) ),
	( _BV ( PD2 ) | _BV ( PD3 ) ),
	( _BV ( PD3 ) | _BV ( PD0 ) ),
};

ISR ( TIMER1_COMPA_vect ) {
	static uint8_t position;
	uint8_t buttons;

	/* Read buttons and update position */
	buttons = PINB;
	if ( ! ( buttons & _BV ( PB2 ) ) )
		position++;
	if ( ! ( buttons & _BV ( PB1 ) ) )
		position--;

	/* Set outputs according to current position */
	PORTD = outputs [ position % sizeof ( outputs ) ];
}

int main ( void ) {

	/* Configure input pins with internal pull-up resistors */
	PORTB = ( _BV ( PB1 ) | _BV ( PB2 ) );

	/* Configure output pins */
	DDRD = ( _BV ( PD0 ) | _BV ( PD1 ) | _BV ( PD2 ) | _BV ( PD3 ) );

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
