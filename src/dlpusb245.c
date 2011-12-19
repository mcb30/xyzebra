#include <stdint.h>
#include <stdio.h>
#include <libusb.h>
#include "dlpusb245.h"

/** FTDI USB vendor ID */
#define VENDOR_ID 0x0403

/** FTDI USB device ID */
#define DEVICE_ID 0x6001

/** FTDI bulk output endpoint */
#define ENDPOINT_OUT 0x02

/** FTDI bulk input endpoint */
#define ENDPOINT_IN 0x81

/** FTDI USB request type for output control transfers */
#define REQUEST_TYPE_OUT ( LIBUSB_REQUEST_TYPE_VENDOR | \
			   LIBUSB_RECIPIENT_DEVICE |	\
			   LIBUSB_ENDPOINT_OUT )

/** FTDI USB request type for input control transfers */
#define REQUEST_TYPE_IN ( LIBUSB_REQUEST_TYPE_VENDOR | \
			  LIBUSB_RECIPIENT_DEVICE |    \
			  LIBUSB_ENDPOINT_IN )

/** FTDI USB request ID for setting bit mode */
#define REQUEST_SET_BITMODE 0x0b

/** FTDI USB request ID for reading pin status */
#define REQUEST_READ_PINS 0x0c

/** FTDI mode for bit banging */
#define BITMODE_BITBANG 0x01

/** FTDI mode for FIFO operation */
#define BITMODE_FIFO 0x00

/** FTDI interface number */
#define PORT_INTERFACE 0x01

/** Timeout for USB operations (in ms) */
#define TIMEOUT_MS 5000

/** Print an error message */
#define eprintf(...) fprintf ( stderr, __VA_ARGS__ )

/**
 * Open DLPUSB245 device
 *
 * @v device		DLPUSB245 device
 * @ret lusbrc		0 on success, or LIBUSB_ERROR code on failure
 */
int open_device ( struct dlpusb245_device *device ) {
	int lusbrc;

	/* Initialise libusb */
	if ( ( lusbrc = libusb_init ( &device->context ) ) < 0 ) {
		eprintf ( "Could not initialise libusb: error %d\n", lusbrc );
		goto err_libusb_init;
	}

	/* Open device */
	device->device = libusb_open_device_with_vid_pid ( device->context,
							   VENDOR_ID,
							   DEVICE_ID );
	if ( ! device->device ) {
		eprintf ( "Could not open USB device %04x:%04x\n",
			  VENDOR_ID, DEVICE_ID );
		lusbrc = LIBUSB_ERROR_IO;
		goto err_libusb_open_device;
	}

	/* Set output mask to all-outputs by default */
	if ( ( lusbrc = set_output_mask ( device, 0xff, 1 ) ) < 0 )
		goto err_set_output_mask;

	return 0;

 err_set_output_mask:
	libusb_close ( device->device );
 err_libusb_open_device:
	libusb_exit ( device->context );
 err_libusb_init:
	return lusbrc;
}

/**
 * Close DLPUSB245 device
 *
 * @v device		DLPUSB245 device
 */
void close_device ( struct dlpusb245_device *device ) {
	libusb_close ( device->device );
	libusb_exit ( device->context );
}

/**
 * Set output mask
 *
 * @v device		DLPUSB245 device
 * @v output_mask	Output mask
 * @v bitbang		Use bit-banging mode
 * @ret lusbrc		0 on success, or LIBUSB_ERROR code on failure
 */
int set_output_mask ( struct dlpusb245_device *device, uint8_t output_mask,
		      int bitbang ) {
	uint16_t value;
	uint8_t mode;
	int lusbrc;

	/* Construct control value */
	mode = ( bitbang ? BITMODE_BITBANG : BITMODE_FIFO );
	value = ( ( mode << 8 ) | output_mask );
	if ( ( lusbrc = libusb_control_transfer ( device->device,
						  REQUEST_TYPE_OUT,
						  REQUEST_SET_BITMODE, value,
						  PORT_INTERFACE, NULL, 0,
						  TIMEOUT_MS ) ) < 0 ) {
		eprintf ( "Could not set output mask: %d\n", lusbrc );
		return lusbrc;
	}

	return 0;
}

/**
 * Set output value
 *
 * @v device		DLPUSB245 device
 * @v output_value	Output value
 * @ret lusbrc		0 on success, or LIBUSB_ERROR code on failure
 */
int set_output_value ( struct dlpusb245_device *device, uint8_t output_value ) {
	uint8_t data[1];
	int count;
	int lusbrc;

	/* Write data */
	data[0] = output_value;
	if ( ( lusbrc = libusb_bulk_transfer ( device->device, ENDPOINT_OUT,
					       data, sizeof ( data ), &count,
					       TIMEOUT_MS ) ) < 0 ) {
		eprintf ( "Could not set outputs: %d\n", lusbrc );
		return lusbrc;
	}

	/* Check that correct amount of data was written */
	if ( count != sizeof ( data ) ) {
		eprintf ( "Wrote incomplete data (%d of %zd)\n",
			  count, sizeof ( data ) );
		return LIBUSB_ERROR_IO;
	}

	return 0;
}

/**
 * Read input value
 *
 * @v device		DLPUSB245 device
 * @v input_value	Input value to fill in
 * @ret lusbrc		0 on success, or LIBUSB_ERROR code on failure
 */
int read_input_value ( struct dlpusb245_device *device, uint8_t *input_value ) {
	uint8_t data[1];
	int lusbrc;

	/* Read data */
	if ( ( lusbrc = libusb_control_transfer ( device->device,
						  REQUEST_TYPE_IN,
						  REQUEST_READ_PINS, 0,
						  PORT_INTERFACE,
						  data, sizeof ( data ),
						  TIMEOUT_MS ) ) < 0 ) {
		eprintf ( "Could not read pins: %d\n", lusbrc );
		return lusbrc;
	}

	/* Return value read from pins */
	*input_value = data[0];

	return 0;
}

/**
 * Write data to FIFO
 *
 * @v device		DLPUSB245 device
 * @v data		Data to write
 * @v len		Length of data
 * @ret lusbrc		0 on success, or LIBUSB_ERROR code on failure
 */
int write_fifo ( struct dlpusb245_device *device, uint8_t *data, size_t len ) {
	size_t written;
	int count;
	int lusbrc;

	/* Write data */
	if ( ( lusbrc = libusb_bulk_transfer ( device->device, ENDPOINT_OUT,
					       data, len, &count,
					       TIMEOUT_MS ) ) < 0 ) {
		eprintf ( "Could not write data: %d\n", lusbrc );
		return lusbrc;
	}

	/* Check that correct amount of data was written */
	written = count;
	if ( written != len ) {
		eprintf ( "Wrote incomplete data (%zd of %zd)\n",
			  written, len );
		return LIBUSB_ERROR_IO;
	}

	return 0;
}
