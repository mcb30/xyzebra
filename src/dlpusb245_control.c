#include <stdlib.h>
#include <stdio.h>
#include "dlpusb245.h"
#include <unistd.h>
#include <libusb.h>

struct stepper_pattern {
	uint8_t bits[4];
};

struct stepper_pattern full_step_y = {
	.bits = { 0x90, 0xc0, 0x60, 0x30 },
};

struct stepper_pattern full_step_x = {
	.bits = { 0x03, 0x06, 0x0c, 0x09 },
};

static int move_to_target ( struct dlpusb245_device *device,
			    unsigned int x, unsigned int y,
			    unsigned int target_x, unsigned int target_y ) {
	unsigned int bits;
	int lusbrc;

	while ( ( x != target_x ) || ( y != target_y ) ) {
		
		bits = ( full_step_y.bits [ y % 4 ] |
			 full_step_x.bits [ x % 4 ] );

		if ( ( lusbrc = set_output_value ( device, bits ) ) < 0 )
			return lusbrc;
		
		usleep ( 1000 );
			
		if ( x < target_x ) {
			x++;
		} else if ( x > target_x ) {
			x--;
		}
			
		if ( y < target_y ) {
			y++;
		} else if ( y > target_y ) {
			y--;
		}
			
	}

	return 0;
}

static int move_to_targets ( struct dlpusb245_device *device,
			     unsigned int original_x,
			     unsigned int original_y ) {
	unsigned int x = original_x;
	unsigned int y = original_y;
	unsigned int a_target_x;
	unsigned int a_target_y;
	unsigned int target_x;
	unsigned int target_y;
	int lusbrc;

	while ( 1 ) {

		if ( scanf ( "%d,%d", &a_target_x, &a_target_y ) != 2 ) {
			printf ( "Invalid co-ordinate\n" );
			return LIBUSB_ERROR_IO;
		}

		target_x = a_target_x*1000;
		target_y = a_target_y*1000;
		printf ( "Aiming for %d,%d\n", target_x, target_y );

		if ( ( lusbrc = move_to_target ( device, x, y,
						 target_x, target_y ) ) < 0 ) {
			return lusbrc;
		}
		
		x = target_x;
		y = target_y;
	}
}
	
int main ( void ) {
	struct dlpusb245_device device;
	int lusbrc;
	
	/* Open device */
	if ( ( lusbrc = open_device ( &device ) ) < 0 )
		goto err_open_device;

#if 0
	if ( ( lusbrc = move_to_target ( &device, 20000, 20000, 0, 0 ) ) < 0 )
		goto err_move_to_target;

	if ( ( lusbrc = move_to_target ( &device, 0, 0, 6000, 0 ) ) < 0 )
		goto err_move_to_target;
#endif

	if ( ( lusbrc = move_to_targets ( &device, 0, 0 ) ) < 0 )
		goto err_move_to_target;

	/* Close device */
	close_device ( &device );
	
	/* Exit */
	exit ( EXIT_SUCCESS );
	
 err_move_to_target:
	close_device ( &device );
 err_open_device:
	exit ( EXIT_FAILURE );
}
