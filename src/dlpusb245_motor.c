#include <stdlib.h>
#include "dlpusb245.h"
#include <unistd.h>
/**
 * Main program
 *
 */
int main ( void ) {
	struct dlpusb245_device device;
	int lusbrc;

	unsigned int i;
	unsigned int j;

	unsigned int my_outputs[] = {
		0, 1, 3, 6, 12, 8, 0, 8, 12, 6, 3, 1, 0
	};

	/* Open device */
	if ( ( lusbrc = open_device ( &device ) ) < 0 )
		goto err_open_device;
	

	for ( j=0; j<12; j++ ) {
		for ( i=0; i<100; i+=10 ) {
			/* Set outputs to on */
			if ( ( lusbrc = set_output_value ( &device, my_outputs[j+1] ) ) < 0 )
				goto err_set_output_value;

			/* Wait */
			usleep ( 100*i );

			/* Set outputs to off */
			if ( ( lusbrc = set_output_value ( &device, my_outputs[j] ) ) < 0 )
				goto err_set_output_value;
		
			/* Wait */
			usleep ( 100*( 100-i ) );
		}
	}

	/* Set outputs to off */
	if ( ( lusbrc = set_output_value ( &device, 0 ) ) < 0 )
		goto err_set_output_value;
	
	/* Close device */
	close_device ( &device );
	
	/* Exit */
	exit ( EXIT_SUCCESS );
	
 err_set_output_value:
			close_device ( &device );
 err_open_device:
			exit ( EXIT_FAILURE );
}
