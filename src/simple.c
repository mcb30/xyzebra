#include <stdlib.h>
#include "dlpusb245.h"

/**
 * Do something simple
 *
 * @v device		DLPUSB245 device
 * @ret lusbrc		0 on success, or LIBUSB_ERROR code on failure
 */
static int something_simple ( struct dlpusb245_device *device ) {
	int lusbrc;

	/* Set outputs on */
	if ( ( lusbrc = set_output_value ( device, 0x55 ) ) < 0 )
		return lusbrc;

	return 0;
}

/**
 * Main program
 */
int main ( void ) {
	struct dlpusb245_device device;
	int lusbrc;

	/* Open device */
	if ( ( lusbrc = open_device ( &device ) ) < 0 )
		goto err_open_device;

	/* Do something simple */
	if ( ( lusbrc = something_simple ( &device ) ) < 0 )
		goto err_something_simple;

	/* Close device */
	close_device ( &device );

	/* Exit */
	exit ( EXIT_SUCCESS );

 err_something_simple:
	close_device ( &device );
 err_open_device:
	exit ( EXIT_FAILURE );
}
