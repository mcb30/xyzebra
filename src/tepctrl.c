#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define TIMER_THRESHOLD 0x200

#define BUTTON_PINS PINB
#define BUTTON_LEFT PB1
#define BUTTON_RIGHT PB2
#define BUTTON_UP PB7
#define BUTTON_DOWN PB0
#define LED_X0 PD0
#define LED_X1 PD1
#define LED_X2 PD2
#define LED_X3 PD3
#define LED_Y0 PC0
#define LED_Y1 PC1
#define LED_Y2 PC2
#define LED_Y3 PC3
#define OUT_X PORTD
#define OUT_Y PORTC
#define IN_BUTTONS PORTB
#define OUT_PINS_X DDRD
#define OUT_PINS_Y DDRC

static const uint8_t x_outputs[] = {
	( _BV ( LED_X0 ) | _BV ( LED_X1 ) ),
	( _BV ( LED_X1 ) | _BV ( LED_X2 ) ),
	( _BV ( LED_X2 ) | _BV ( LED_X3 ) ),
	( _BV ( LED_X3 ) | _BV ( LED_X0 ) ),
};

static const uint8_t y_outputs[] = {
	( _BV ( LED_Y0 ) | _BV ( LED_Y1 ) ),
	( _BV ( LED_Y1 ) | _BV ( LED_Y2 ) ),
	( _BV ( LED_Y2 ) | _BV ( LED_Y3 ) ),
	( _BV ( LED_Y3 ) | _BV ( LED_Y0 ) ),
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

	if ( ! ( buttons & _BV ( BUTTON_UP ) ) )
		y_position++;
	if ( ! ( buttons & _BV ( BUTTON_DOWN ) ) )
		y_position--;

	/* Set outputs according to current position */
	OUT_X = x_outputs [ x_position % sizeof ( x_outputs ) ];
	OUT_Y = y_outputs [ y_position % sizeof ( y_outputs ) ];
}

int main ( void ) {

	/* Configure input pins with internal pull-up resistors */
	IN_BUTTONS = ( _BV ( BUTTON_LEFT ) | _BV ( BUTTON_RIGHT ) | _BV ( BUTTON_UP )| 
		  _BV ( BUTTON_DOWN ) );

	/* Configure output pins */
	OUT_PINS_X = ( _BV ( LED_X0 ) | _BV ( LED_X1 ) | _BV ( LED_X2 ) | 
		       _BV ( LED_X3 ) );
	OUT_PINS_Y = ( _BV ( LED_Y0 ) | _BV ( LED_Y1 ) | _BV ( LED_Y2 ) | 
		       _BV ( LED_Y3 ) );

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
